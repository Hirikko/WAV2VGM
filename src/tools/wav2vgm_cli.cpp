
#include "AudioProject.h"
#include "OplFit.h"
#include "VgmWriter.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static void PrintHelp()
{
    std::cout <<
        "WAV2VGM CLI - WAV to OPL3 VGM converter\n\n"
        "Usage:\n"
        "  wav2vgm-cli [options] file1.wav [file2.wav ...]\n\n"
        "Options:\n"
        "  -o, --output DIR     Output directory (default: input directory)\n"
        "  --fps NUMBER        Analysis frame rate (default: 100)\n"
        "  --channels NUMBER   Maximum OPL3 channels (default: maximum)\n"
        "  --help              Show this help\n\n"
        "Examples:\n"
        "  wav2vgm-cli song.wav\n"
        "  wav2vgm-cli -o output/ song1.wav song2.wav\n"
        "  wav2vgm-cli --fps 100 --channels 18 *.wav\n";
}

static bool ConvertFile(
    const fs::path& input,
    const fs::path& outputDir,
    double fps,
    int maxChannels)
{
    std::cout << "\n========================================\n";
    std::cout << "Input: " << input << "\n";

    AudioProject project;
    std::string error;

    if (!project.ImportFromFile(
            input.string(),
            error,
            [](int percent, const std::string& stage)
            {
                std::cout << "\r" << stage << ": "
                          << percent << "%   " << std::flush;
            }))
    {
        std::cerr << "\nERROR loading WAV: " << error << "\n";
        return false;
    }

    std::cout << "\nSample rate: " << project.SampleRate()
              << " Hz\n";

    if (project.Samples().empty())
    {
        std::cerr << "ERROR: WAV contains no samples.\n";
        return false;
    }

    std::cout << "Analyzing and fitting OPL3 channels...\n";

    auto fit = oplfit::FitChannelsInTurn(
        project.Samples(),
        project.SampleRate(),
        fps,
        maxChannels,
        [](int percent)
        {
            std::cout << "\rFitting: "
                      << percent << "%   " << std::flush;
        });

    std::cout << "\nGenerated channels: "
              << fit.channels.size() << "\n";

    if (fit.channels.empty())
    {
        std::cerr << "ERROR: No OPL3 channels generated.\n";
        return false;
    }

    std::error_code ec;
    fs::create_directories(outputDir, ec);

    if (ec)
    {
        std::cerr << "ERROR creating output directory: "
                  << ec.message() << "\n";
        return false;
    }

    fs::path output = outputDir / input.filename();
    output.replace_extension(".vgm");

    if (!vgm::WriteVgmFile(output.string(), fit.channels, error))
    {
        std::cerr << "ERROR writing VGM: " << error << "\n";
        return false;
    }

    std::cout << "SUCCESS: " << output << "\n";
    return true;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        PrintHelp();
        return 1;
    }

    double fps = oplfit::kDefaultInteractiveFrameRateHz;
    int channels = oplfit::kDefaultMaxChannels;

    fs::path outputDir;
    bool customOutput = false;

    std::vector<fs::path> inputs;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h")
        {
            PrintHelp();
            return 0;
        }
        else if (arg == "-o" || arg == "--output")
        {
            if (++i >= argc)
            {
                std::cerr << "ERROR: Missing output directory.\n";
                return 1;
            }

            outputDir = argv[i];
            customOutput = true;
        }
        else if (arg == "--fps")
        {
            if (++i >= argc)
            {
                std::cerr << "ERROR: Missing FPS value.\n";
                return 1;
            }

            try
            {
                fps = std::stod(argv[i]);
            }
            catch (...)
            {
                std::cerr << "ERROR: Invalid FPS value.\n";
                return 1;
            }

            if (fps <= 0.0)
            {
                std::cerr << "ERROR: FPS must be positive.\n";
                return 1;
            }
        }
        else if (arg == "--channels")
        {
            if (++i >= argc)
            {
                std::cerr << "ERROR: Missing channel count.\n";
                return 1;
            }

            try
            {
                channels = std::stoi(argv[i]);
            }
            catch (...)
            {
                std::cerr << "ERROR: Invalid channel count.\n";
                return 1;
            }

            if (channels < oplfit::kMinMaxChannels ||
                channels > oplfit::kMaxMaxChannels)
            {
                std::cerr << "ERROR: Channel count must be between "
                          << oplfit::kMinMaxChannels << " and "
                          << oplfit::kMaxMaxChannels << ".\n";
                return 1;
            }
        }
        else if (!arg.empty() && arg[0] == '-')
        {
            std::cerr << "ERROR: Unknown option: " << arg << "\n";
            return 1;
        }
        else
        {
            inputs.emplace_back(arg);
        }
    }

    if (inputs.empty())
    {
        std::cerr << "ERROR: No WAV files specified.\n";
        return 1;
    }

    int succeeded = 0;
    int failed = 0;

    for (const auto& input : inputs)
    {
        if (!fs::exists(input) || !fs::is_regular_file(input))
        {
            std::cerr << "\nERROR: File not found: "
                      << input << "\n";
            ++failed;
            continue;
        }

        if (input.extension() != ".wav" &&
            input.extension() != ".WAV")
        {
            std::cerr << "\nSkipping non-WAV file: "
                      << input << "\n";
            continue;
        }

        fs::path destination = customOutput
            ? outputDir
            : input.parent_path();

        if (ConvertFile(input, destination, fps, channels))
            ++succeeded;
        else
            ++failed;
    }

    std::cout << "\n========================================\n";
    std::cout << "Conversion finished.\n";
    std::cout << "Successful: " << succeeded << "\n";
    std::cout << "Failed:     " << failed << "\n";

    return failed > 0 ? 1 : 0;
}
