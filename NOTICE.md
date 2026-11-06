
# Componentes de terceiros

## dbopl (Emulador OPL3 / YMF262)

O diretório `src/third_party/dbopl/` (`dbopl.h`, `dbopl.cpp`) contém o núcleo do emulador OPL3 do projeto [DOSBox](https://www.dosbox.com/), protegido por direitos autorais © 2002-2020 The DOSBox Team, extraído de forma independente por rofl0r e disponibilizado em:

<https://github.com/rofl0r/dbopl>

O componente é utilizado sem modificações para fornecer a funcionalidade de emulação do modo de análise "OPL3".

**Licença: GNU General Public License versão 2 ou posterior (GPL2+)**. O texto integral da licença está disponível no arquivo [`LICENSE-GPL2.txt`](LICENSE-GPL2.txt), localizado na raiz do repositório.

Os cabeçalhos originais de direitos autorais e licenciamento foram preservados integralmente no início de ambos os arquivos incorporados ao projeto (`dbopl.h` e `dbopl.cpp`).

Como o dbopl é licenciado sob GPL2+ e está vinculado estaticamente ao executável `WAV2VGM` (não sendo executado em um processo separado nem carregado dinamicamente como um componente independente), **qualquer compilação do WAV2VGM que inclua o modo de análise OPL3 constitui uma obra combinada sujeita à GPL2+ como um todo.**

Caso uma compilação desse tipo seja distribuída, é necessário cumprir as exigências da GPL2+ para todo o binário distribuído, e não apenas para a parte correspondente ao dbopl. Isso inclui disponibilizar o código-fonte correspondente e preservar os avisos de direitos autorais e licenciamento.

Essas obrigações de distribuição não se aplicam ao uso exclusivamente local e não distribuído do programa.
