
#!/usr/bin/env bash

set -e

# WAV2VGM - Linux dependency checker and installer

MODE="${1:---gui}"

case "$MODE" in
    --gui|--cli) ;;
    *)
        echo "Usage: $0 [--gui|--cli]"
        exit 1
        ;;
esac

if [[ ! -f /etc/os-release ]]; then
    echo "Error: Cannot identify Linux distribution."
    exit 1
fi

source /etc/os-release

echo "======================================"
echo " WAV2VGM - Dependency Checker"
echo "======================================"
echo "Distribution: ${PRETTY_NAME}"
echo "Build mode: ${MODE#--}"
echo

MISSING=()

# Check common build tools
for cmd in cmake c++; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        MISSING+=("$cmd")
    else
        echo "[OK] $cmd"
    fi
done

# Check build system
if ! command -v make >/dev/null 2>&1 && \
   ! command -v ninja >/dev/null 2>&1; then
    MISSING+=("make")
else
    echo "[OK] Build system"
fi

# Check wxWidgets only for GUI builds
if [[ "$MODE" == "--gui" ]]; then
    if command -v wx-config >/dev/null 2>&1; then
        echo "[OK] wxWidgets ($(wx-config --version))"
    else
        MISSING+=("wxWidgets")
    fi
fi

if [[ ${#MISSING[@]} -eq 0 ]]; then
    echo
    echo "======================================"
    echo " All dependencies are installed!"
    echo " No installation required."
    echo "======================================"
    exit 0
fi

echo
echo "Missing dependencies:"
printf ' - %s\n' "${MISSING[@]}"
echo

# Select distribution packages
case "$ID $ID_LIKE" in
    *ubuntu*|*debian*)
        MANAGER="apt"
        BUILD_PACKAGES="build-essential cmake"
        GUI_PACKAGES="libwxgtk3.2-dev"
        ;;
    *fedora*)
        MANAGER="dnf"
        BUILD_PACKAGES="gcc-c++ cmake make"
        GUI_PACKAGES="wxGTK-devel"
        ;;
    *arch*)
        MANAGER="pacman"
        BUILD_PACKAGES="base-devel cmake"
        GUI_PACKAGES="wxwidgets-gtk3"
        ;;
    *opensuse*|*suse*)
        MANAGER="zypper"
        BUILD_PACKAGES="gcc-c++ cmake make"
        GUI_PACKAGES="wxWidgets-3_2-devel"
        ;;
    *)
        echo "Unsupported distribution: $ID"
        echo "Please install the missing dependencies manually."
        exit 1
        ;;
esac

PACKAGES="$BUILD_PACKAGES"

if [[ "$MODE" == "--gui" ]]; then
    PACKAGES="$PACKAGES $GUI_PACKAGES"
fi

echo "The following packages may be installed:"
echo "$PACKAGES"
echo

read -r -p "Proceed with installation? [y/N] " ANSWER

case "$ANSWER" in
    y|Y|yes|YES|s|S|sim|SIM) ;;
    *)
        echo "Installation cancelled."
        exit 0
        ;;
esac

case "$MANAGER" in
    apt)
        sudo apt update
        sudo apt install -y $PACKAGES
        ;;
    dnf)
        sudo dnf install -y $PACKAGES
        ;;
    pacman)
        sudo pacman -S --needed $PACKAGES
        ;;
    zypper)
        sudo zypper install -y $PACKAGES
        ;;
esac

echo
echo "Installation completed."
echo "Run the dependency checker again to verify."
