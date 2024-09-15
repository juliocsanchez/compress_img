#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_GRAY 255

// Estrutura para armazenar a imagem
typedef struct {
    char type[3];
    int width;
    int height;
    int max_gray;
    unsigned char **pixels;
} PGMImage;

// Função para criar uma imagem PGM vazia
PGMImage* createPGM(int width, int height, int max_gray) {
    PGMImage *img = (PGMImage *)malloc(sizeof(PGMImage));
    img->width = width;
    img->height = height;
    img->max_gray = max_gray;

    // Alocar memória para os pixels
    img->pixels = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }

    return img;
}

// Função para salvar a imagem PGM
void savePGM(PGMImage *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erro ao salvar o arquivo.\n");
        return;
    }

    // Verificar o tipo da imagem e salvar de acordo
    if (img->type[1] == '2') {
        // Formato P2 (ASCII)
        fprintf(file, "P2\n%d %d\n%d\n", img->width, img->height, img->max_gray);
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                fprintf(file, "%d ", img->pixels[i][j]);
            }
            fprintf(file, "\n");
        }
    } else if (img->type[1] == '5') {
        // Formato P5 (Binário)
        fprintf(file, "P5\n%d %d\n%d\n", img->width, img->height, img->max_gray);
        for (int i = 0; i < img->height; i++) {
            fwrite(img->pixels[i], sizeof(unsigned char), img->width, file);
        }
    }

    fclose(file);
}

// Função para preencher uma região com um valor de cinza
void fillRegion(PGMImage *img, int x, int y, int size, unsigned char value) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            img->pixels[y + i][x + j] = value;
        }
    }
}

// Função recursiva para decodificar o bitstream e reconstruir a imagem
void decodeBitstreamToImage(PGMImage *img, FILE *bitstream, int x, int y, int size) {
    int flag = fgetc(bitstream);  // Lê o próximo bit do arquivo (0 ou 1)

    if (flag == 1) {
        // Região homogênea: lê o valor do tom de cinza
        unsigned char gray_value = fgetc(bitstream);
        fillRegion(img, x, y, size, gray_value);  // Preenche a região com o tom de cinza
    } else if (flag == 0) {
        // Região não homogênea: divide em 4 quadrantes e continua a decodificação
        int half = size / 2;
        decodeBitstreamToImage(img, bitstream, x, y, half);              // Quadrante superior esquerdo
        decodeBitstreamToImage(img, bitstream, x + half, y, half);       // Quadrante superior direito
        decodeBitstreamToImage(img, bitstream, x, y + half, half);       // Quadrante inferior esquerdo
        decodeBitstreamToImage(img, bitstream, x + half, y + half, half); // Quadrante inferior direito
    }
}

// Função principal para decodificar o bitstream e reconstruir a imagem
void decodeBitstream(const char *bitstream_filename, const char *output_filename) {
    FILE *bitstream = fopen(bitstream_filename, "rb");
    if (!bitstream) {
        printf("Erro ao abrir o arquivo de bitstream.\n");
        return;
    }

    // Lê o tipo de arquivo como um valor binário
    unsigned char type_code;
    fread(&type_code, sizeof(unsigned char), 1, bitstream);

    // Lê a largura e a altura do bitstream
    int width, height;
    fread(&width, sizeof(int), 1, bitstream);
    fread(&height, sizeof(int), 1, bitstream);

    PGMImage *img = createPGM(width, height, MAX_GRAY);

    // Mapear o valor binário de volta para 'P2' ou 'P5'
    if (type_code == 0x02) {
        img->type[0] = 'P';
        img->type[1] = '2';
    } else if (type_code == 0x05) {
        img->type[0] = 'P';
        img->type[1] = '5';
    }
    img->type[2] = '\0';

    // Começa a decodificar o bitstream e reconstruir a imagem
    int size = width > height ? width : height; // Tamanho máximo
    decodeBitstreamToImage(img, bitstream, 0, 0, size);

    fclose(bitstream);

    // Salva a imagem decodificada como um arquivo PGM
    savePGM(img, output_filename);

    // Libera a memória da imagem
    for (int i = 0; i < img->height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_bitstream> <arquivo_saida.pgm>\n", argv[0]);
        return 1;
    }

    decodeBitstream(argv[1], argv[2]);

    return 0;
}
