#include "../lib/prototypes.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_bitstream.bin> <arquivo_saida.pgm>\n", argv[0]);
        return 1;
    }

    decodeBitstream(argv[1], argv[2]);

    return 0;
}
