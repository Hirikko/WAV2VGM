# WAV2VGM

Aplicativo multiplataforma desenvolvido em C++ que possui interface gráfica (GUI) e conversor por linha de comando (CLI). Importa arquivos de áudio, normaliza para mono em 44,1 kHz e realiza uma aproximação utilizando configurações reais de registradores de chips sintetizadores — atualmente um sintetizador FM OPL3 (YMF262) — permitindo exportar arquivos VGM padrão, reproduzíveis em hardware real ou em qualquer reprodutor compatível com VGM.

## Galeria

### WAV original

!WAV original

### Aproximação OPL3

!Aproximação OPL3

### Análise de picos espectrais

!Análise de picos

## Tecnologias utilizadas

- C++17
- CMake 3.21+
- wxWidgets 3.2+
- Interface gráfica multiplataforma com wxWidgets
- Emulador OPL3 (dbopl, do projeto DOSBox)

## Funcionalidades

- Janela principal com menu Arquivo: Importar WAV e Sair.
- Janelas de projetos independentes (MDI), com abas para cada modo de análise: "OPL3" (selecionado por padrão) e "Peak Trends".
- Importação de arquivos WAV com conversão para mono, reamostragem para 44,1 kHz, normalização de amplitude e geração de espectrograma STFT.
- **Modo OPL3:** aproxima o áudio utilizando até 18 canais reais do OPL3, renderiza o resultado através de um emulador OPL3 e permite exportar arquivos VGM padrão.
- **Modo Peak Trends:** acompanha picos espectrais em linhas de tendência persistentes ao longo da gravação.
- Zoom e rolagem do eixo temporal sincronizados entre as abas.
- Configurações persistentes entre execuções em `WAV2VGM.ini`, incluindo os últimos diretórios de importação/exportação e parâmetros do espectrograma.
- **Conversor CLI:** converte um ou vários arquivos WAV para VGM diretamente pelo terminal.
- Configuração da taxa de análise (`--fps`) e da quantidade máxima de canais OPL3 (`--channels`).
- **Compilação modular:** possibilidade de compilar somente a interface gráfica, somente o conversor CLI ou ambos.
- **Verificação automática de dependências no Linux:** identifica a distribuição e verifica as ferramentas e bibliotecas necessárias.
- **Instalação inteligente de dependências:** verifica o que já está instalado e solicita instalação somente quando necessário.
- **Instalação permanente:** disponibiliza os executáveis no sistema através do mecanismo de instalação do CMake.

## Compilação e instalação no Linux

O WAV2VGM permite duas modalidades independentes de compilação:

- **GUI:** aplicativo completo com interface gráfica, que utiliza wxWidgets.
- **CLI:** conversor de arquivos WAV para VGM pela linha de comando, sem necessidade de wxWidgets.

### Distribuições Linux contempladas

O instalador de dependências identifica a distribuição Linux e utiliza seu gerenciador de pacotes nativo.

|Distribuição|Gerenciador de pacotes|
|---|---|
|Ubuntu / Debian e derivados|APT|
|Fedora|DNF|
|Arch Linux e derivados, incluindo CachyOS|Pacman|
|openSUSE|Zypper|

### 1. Verificar e instalar dependências

Antes de compilar, execute o script de verificação de dependências.

Para a aplicação completa (GUI + CLI):

bash

```
./scripts/install-deps.sh --gui
```

Para compilar somente o conversor CLI:

bash

```
./scripts/install-deps.sh --cli
```

O script realiza as seguintes operações:

- Identifica automaticamente a distribuição Linux.
- Verifica se o CMake está instalado.
- Verifica se existe um compilador C++ disponível.
- Verifica a disponibilidade das ferramentas de compilação.
- Verifica o wxWidgets quando a interface gráfica é solicitada.
- Informa quais dependências já estão instaladas.
- Identifica os pacotes ausentes.
- Solicita autorização antes de instalar qualquer pacote.
- Utiliza o gerenciador de pacotes nativo da distribuição.

Se todas as dependências necessárias já estiverem disponíveis, nenhuma instalação será solicitada.

### 2. Compilar a aplicação completa (GUI + CLI)

Na raiz do projeto, execute:

bash

```
cmake -S . -B build
cmake --build build -j$(nproc)
```

### 3. Compilar somente o conversor CLI

Não é necessário instalar wxWidgets para compilar somente o conversor por linha de comando.

bash

```
cmake -S . -B build-cli -DWAV2VGM_BUILD_GUI=OFF
cmake --build build-cli -j$(nproc)
```

### 4. Compilar somente a interface gráfica

bash

```
cmake -S . -B build-gui -DWAV2VGM_BUILD_CLI=OFF
cmake --build build-gui -j$(nproc)
```

### 5. Instalação permanente

Após compilar, instale os executáveis no sistema:

bash

```
sudo cmake --install build
```

Por padrão, os programas são instalados em:

text

```
/usr/local/bin/
```

Executáveis instalados:

- `WAV2VGM` — aplicativo com interface gráfica.
- `wav2vgm-cli` — conversor pela linha de comando.

Após a instalação, os programas podem ser executados de qualquer diretório, sem precisar acessar a pasta de compilação.

### 6. Utilizando o conversor CLI

Converter um único arquivo WAV:

bash

```
wav2vgm-cli musica.wav
```

Converter vários arquivos WAV:

bash

```
wav2vgm-cli -o vgms/ *.wav
```

Definir a taxa de análise e a quantidade máxima de canais OPL3:

bash

```
wav2vgm-cli --fps 100 --channels 18 *.wav
```

Consultar todas as opções disponíveis:

bash

```
wav2vgm-cli --help
```

#### Opções disponíveis

text

```
WAV2VGM CLI - Conversor WAV para OPL3 VGM

Uso:
  wav2vgm-cli [opções] arquivo1.wav [arquivo2.wav ...]

Opções:
  -o, --output DIR     Diretório de saída (padrão: diretório de entrada)
  --fps NUMBER         Taxa de análise (padrão: 100)
  --channels NUMBER    Quantidade máxima de canais OPL3 (padrão: máximo)
  --help               Exibe esta ajuda
```

### 7. Desinstalação

Para remover os executáveis instalados:

bash

```
sudo rm /usr/local/bin/WAV2VGM
sudo rm /usr/local/bin/wav2vgm-cli
```

A desinstalação não remove os arquivos do projeto nem os arquivos de configuração do usuário.

## Compilação no Windows

A ferramenta de compilação utilizada é **MSVC + vcpkg**.

### 1. Instalar o Visual Studio

Instale o **Visual Studio 2022 ou superior**, incluindo a carga de trabalho:

`Desenvolvimento para desktop com C++`

A instalação inclui o compilador MSVC, CMake, Ninja e vcpkg.

### 2. Configurar o VCPKG_ROOT

Abra o **Developer PowerShell for VS 2022** e configure a variável de ambiente:

powershell

```
$env:VCPKG_ROOT = "C:\Program Files\Microsoft Visual Studio\<versão>\Community\VC\vcpkg"
```

Essa configuração deve ser realizada em cada sessão do terminal ou adicionada permanentemente às variáveis de ambiente.

### 3. Configurar e compilar

Na raiz do projeto:

powershell

```
cmake --preset windows
cmake --build --preset windows-debug
```

### 4. Executar

text

```
build/WAV2VGM.exe
```

Na primeira configuração, o vcpkg compila o wxWidgets e suas dependências. Nas configurações seguintes, os pacotes já compilados são reutilizados do cache.

## Executando os testes

O projeto possui dois executáveis de teste para validar os principais componentes e algoritmos.

### dsp_selftest

Valida o processamento FFT, janelas, pipeline dBFS e gradiente de cores utilizando uma onda senoidal sintética.

Executar diretamente:

bash

```
./build/dsp_selftest
```

Ou pelo CTest, dentro da pasta de compilação:

bash

```
cd build
ctest
```

### opl3_selftest

Valida a integração do emulador OPL3, os algoritmos de ajuste de canais, a renderização de áudio e a exportação VGM.

Os testes incluem:

- Geração e renderização de configurações de canais OPL3.
- Validação de frequências através de análise FFT.
- Ajuste de canais por quadro e múltiplos canais.
- Direcionamento por resíduo e alocação de canais.
- Comparação entre renderização incremental e simulação conjunta do OPL3.
- Validação do cabeçalho VGM, escritas nos registradores e terminador do arquivo.

Executar diretamente:

bash

```
./build/opl3_selftest
```

## Componentes de terceiros e licenciamento

Este projeto incorpora `dbopl`, localizado em `src/third_party/dbopl/`.

O dbopl é o núcleo emulador do chip OPL3 (YMF262), extraído do projeto [DOSBox](https://www.dosbox.com/).

Copyright (C) 2002-2020 The DOSBox Team.

O componente é utilizado sem modificações para alimentar o modo de análise OPL3.

**Licença: GNU General Public License versão 2 ou posterior (GPL2+).**

O texto completo da licença está disponível em `LICENSE-GPL2.txt`.

Como o dbopl é vinculado estaticamente ao executável, qualquer versão distribuída que contenha a funcionalidade OPL3 constitui uma obra combinada sob GPL2+ como um todo.

Ao distribuir versões compiladas, é necessário cumprir os requisitos da GPL2+, incluindo disponibilizar o código-fonte correspondente e preservar os avisos de direitos autorais e licenciamento.

Consulte `NOTICE.md` para obter informações adicionais sobre licenciamento.

## Observações e limitações conhecidas

- A importação de MP3 ainda não está implementada. A interface de importação aceita somente arquivos `.wav`.
- Ainda não existe funcionalidade para salvar e carregar projetos. Cada arquivo WAV importado abre uma nova janela de projeto.
- O espectrograma utiliza, por padrão, 4096 amostras por coluna (aproximadamente 92 ms), com um intervalo de 32 amostras entre colunas.
- Os parâmetros FFT e hop podem ser personalizados através do arquivo `WAV2VGM.ini`, utilizando as opções `FftSize` e `HopSize`.
- `FftSize` deve ser uma potência de dois. Valores inválidos no arquivo INI são ignorados e substituídos pelo padrão.
- Ainda não existe uma tela de preferências dentro do aplicativo.
- O conversor CLI atualmente aceita arquivos WAV como entrada e gera arquivos VGM como saída.

## Contribuições

Contribuições são bem-vindas!

Você pode criar um fork do repositório, modificar o código-fonte, aprimorar os algoritmos, adicionar funcionalidades ou adaptar o aplicativo para outros chips sintetizadores.

Ao distribuir versões modificadas, preserve os avisos de licenciamento dos componentes de terceiros e cumpra os requisitos da licença GPL2+.
