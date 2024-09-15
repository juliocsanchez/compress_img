#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_GRAY 255

typedef struct {
    char type[3];
    int width;
    int height;
    int max_gray;
    unsigned char **pixels;
} PGMImage;

PGMImage* allocatePGM(int width, int height, int max_gray) {
    PGMImage *img = (PGMImage *)malloc(sizeof(PGMImage));
    img->width = width;
    img->height = height;
    img->max_gray = max_gray;

    //Alocação de espaço para receber os valores de cada píxel
    img->pixels = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }

    //Retorna a imagem com os espaços a serem preenchidos pelos quadrantes do bitstream
    return img;
}

void headerPGM(PGMImage *img, const char *filename) {

    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //Verifica a possibilidade do arquivo ser P2, e caso seja, escreve o cabeçalho do arquivo
    if (img->type[1] == '2') {
        fprintf(file, "P2\n%d %d\n%d\n", img->width, img->height, img->max_gray);
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                fprintf(file, "%d ", img->pixels[i][j]);
            }
            fprintf(file, "\n");
        }
    // Verifica a possibilidade do arquivo ser P5, e caso seja, escreve o cabeçalho do arquivo
    } else if (img->type[1] == '5') {
        fprintf(file, "P5\n%d %d\n%d\n", img->width, img->height, img->max_gray);
        for (int i = 0; i < img->height; i++) {
            fwrite(img->pixels[i], sizeof(unsigned char), img->width, file); // o 'fwrite' trata especificamente dados binários, como os do arquivo P5
        }
    }

    fclose(file);
}

// Função para preencher o quadrante com a cor especificada no bitstream
void toFill(PGMImage *img, int x, int y, int size, unsigned char value) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            img->pixels[y + i][x + j] = value;
        }
    }
}

// Função recursiva para decodificar o bitstream e reconstruir a imagem
void isHomogeneous(PGMImage *img, FILE *bitstream, int x, int y, int size) {
    // Pega o valor que representa se é homogêneo ou não (1 ou 0)
    int isLeaf = fgetc(bitstream);  

    // Se for homogêneo, preenche com a cor especificada no bitstream
    if (isLeaf == 1) {
        unsigned char grayTone = fgetc(bitstream);
        toFill(img, x, y, size, grayTone);  // Preenche a região com o tom de cinza

    // Se não for homegênea, divide o quadrante
    } else if (isLeaf == 0) {
        int half = size / 2;
        isHomogeneous(img, bitstream, x, y, half); // Quadrante superior esquerdo
        isHomogeneous(img, bitstream, x + half, y, half); // Quadrante superior direito
        isHomogeneous(img, bitstream, x, y + half, half); // Quadrante inferior esquerdo
        isHomogeneous(img, bitstream, x + half, y + half, half); // Quadrante inferior direito
    }
}

void decodeBitstream(const char *fileBin, const char *imgOutput) {

    FILE *readFile = fopen(fileBin, "rb"); // Abrindo o arquivo binário
    if (!readFile) {
        printf("Erro ao abrir o arquivo de bitstream.\n");
        return;
    }

    unsigned char typeFile; // Tipo de arquivo P2 ou P5
    fread(&typeFile, sizeof(unsigned char), 1, readFile);

    //Lendo as dimensões da imagem
    int width, height;
    fread(&width, sizeof(int), 1, readFile);
    fread(&height, sizeof(int), 1, readFile);

    //Criando uma struct para receber as informações
    PGMImage *img = allocatePGM(width, height, MAX_GRAY);

    //Lendo o arquivo e verificando seu tipo de acordo com a tradução em binaŕio
    if (typeFile == 0x02) {
        img->type[0] = 'P';
        img->type[1] = '2';
    } else if (typeFile == 0x05) {
        img->type[0] = 'P';
        img->type[1] = '5';
    }
    img->type[2] = '\0';

    //Como a imagem é quadrada, podemos passar tanto width ou height como argumento
    isHomogeneous(img, readFile, 0, 0, width);

    fclose(readFile);

    // Escreve o cabeçalho do arquivo de saida
    headerPGM(img, imgOutput);

    //Liberação de memória
    for (int i = 0; i < img->height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_bitstream> <arquivo_saida.pgm>\n", argv[0]);
        return 1;
    }

    decodeBitstream(argv[1], argv[2]);

    return 0;
}
