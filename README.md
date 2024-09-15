# Projeto de Codificação e Decodificação de Imagens PGM

Este projeto trata da codificação e decodificação de imagens no formato PGM (Portable Gray Map). Siga as instruções abaixo para realizar os processos de codificação e decodificação utilizando os scripts e ferramentas fornecidas.

## Estrutura do Projeto

- code/funcs/: Contém o Makefile (`tools.mk`) e os arquivos fonte necessários para a codificação da imagem.
- decoder/funcs/: Contém o Makefile (`tools.mk`) e os arquivos fonte necessários para a decodificação da imagem.

---

## Instruções de Uso

### 1. Codificação da Imagem

Navegue até o diretório `code/funcs`:

    cd code/funcs

Compile o código utilizando o Makefile:

    make -f tools.mk

Execute o programa de codificação, fornecendo o arquivo PGM de entrada, o arquivo binário de saída e a tolerância desejada:

    ./main.e <arquivo_de_entrada.pgm> <arquivo_de_saida.bin> <tolerancia>

- `<arquivo_de_entrada.pgm>`: O arquivo de imagem PGM a ser codificado.
- `<arquivo_de_saida.bin>`: O arquivo binário de saída gerado pela codificação.
- `<tolerancia>`: O valor de tolerância a ser usado na codificação.

---

### 2. Decodificação da Imagem

Mova o arquivo `.bin` gerado para o diretório `decoder/funcs`:

    mv <arquivo_de_saida.bin> ../decoder/funcs/

Navegue até o diretório `decoder/funcs`:

    cd ../decoder/funcs

Compile o código utilizando o Makefile:

    make -f tools.mk

Execute o programa de decodificação, fornecendo o arquivo binário de entrada e o arquivo PGM de saída:

    ./main.e <arquivo_de_entrada.bin> <arquivo_de_saida.pgm>

- `<arquivo_de_entrada.bin>`: O arquivo binário gerado pela codificação.
- `<arquivo_de_saida.pgm>`: O arquivo de imagem PGM gerado pela decodificação.

Você pode visualizar o arquivo `.pgm` gerado no diretório após a execução.


Caso encontre problemas ou tenha dúvidas, consulte a documentação adicional ou entre em contato com a equipe de desenvolvimento.
