#include "../lib/prototypes.h"

// Pega o arquivo de entrada e gera o arquivo de saída '.bin' contendo o bitstream 
void pgmToFilebin(PGMImage *img, const char *out_filename, int tolerance) {
    FILE *bitstream = fopen(out_filename, "wb");
    if (!bitstream) {
        printf("Erro ao criar o arquivo de bitstream.\n");
        return;
    }

    // Gravar o tipo do arquivo como um valor binário
    unsigned char typeBin;
    if (img->type[1] == '2') {
        typeBin = 0x02; // Representação binária para P2
    } else if (img->type[1] == '5') {
        typeBin = 0x05; // Representação binária para P5
    } else {
        printf("Formato PGM inválido.\n");
        fclose(bitstream);
        return;
    }

    fwrite(&typeBin, sizeof(unsigned char), 1, bitstream); // Escreve o tipo como binário

    // Grava a largura e altura
    fwrite(&img->width, sizeof(int), 1, bitstream);
    fwrite(&img->height, sizeof(int), 1, bitstream);

    int size = img->width > img->height ? img->width : img->height;
    bitstreamGenerate(img, bitstream, 0, 0, size, tolerance);

    fclose(bitstream);
}

void freePGM(PGMImage *img) {
    for (int i = 0; i < img->height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}
