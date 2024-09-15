/********************************************************************
 * Alunos: Julio Cesar de Sousa Patricio, Marcos Caio Faustino de Paulo
 * Matrículas: 2023204505244, 2023204505252
 * Avaliação 04: Trabalho Final
 * Data: 16.09.2024 - 2024.1 - Prof. Daniel Ferreira
 * Compilador: gcc versão 11.4.0
 ********************************************************************/

#include "../lib/prototypes.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_bitstream.bin> <arquivo_saida.pgm>\n", argv[0]);
        return 1;
    }

    decodeBitstream(argv[1], argv[2]);

    return 0;
}
