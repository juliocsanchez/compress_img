/********************************************************************
 * Alunos: João Guilherme Pimentel Costa, Nicolas Araújo Brito
 * Matrículas: 20232045050490, 20232045050210
 * Avaliação 04: Trabalho Final
 * Data: 16.09.2024 - 2024.1 - Prof. Daniel Ferreira
 * Compilador: gcc versão 11.4.0
 ********************************************************************/
#include "../lib/prototypes.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <arquivo_entrada.pgm> <arquivo_saida.bitstream> <tolerancia>\n", argv[0]);
        return 1;
    }

    int tolerance = atoi(argv[3]); 

    PGMImage *img = loadPGM(argv[1]);
    if (img == NULL) {
        return 1;
    }

    pgmToFilebin(img, argv[2], tolerance);

    freePGM(img);

    return 0;
}
