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
