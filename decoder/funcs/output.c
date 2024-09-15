#include "../lib/prototypes.h"

// Função recursiva para verificar a homegeneidade dos quadrantes
void isHomogeneous(PGMImage *img, FILE *bitstream, int x, int y, int size)
{
    // Pega o valor que representa se é homogêneo ou não (1 ou 0)
    int isLeaf = fgetc(bitstream);

    // Se for homogêneo, preenche com a cor especificada no bitstream
    if (isLeaf == 1)
    {
        unsigned char grayTone = fgetc(bitstream);
        toFill(img, x, y, size, grayTone); // Preenche a região com o tom de cinza

        // Se não for homegênea, divide o quadrante
    }
    else if (isLeaf == 0)
    {
        int half = size / 2;
        isHomogeneous(img, bitstream, x, y, half);               // Quadrante superior esquerdo
        isHomogeneous(img, bitstream, x + half, y, half);        // Quadrante superior direito
        isHomogeneous(img, bitstream, x, y + half, half);        // Quadrante inferior esquerdo
        isHomogeneous(img, bitstream, x + half, y + half, half); // Quadrante inferior direito
    }
}

// Função para
void decodeBitstream(const char *fileBin, const char *imgOutput)
{

    FILE *readFile = fopen(fileBin, "rb"); // Abrindo o arquivo binário
    if (!readFile)
    {
        printf("Erro ao abrir o arquivo de bitstream.\n");
        return;
    }

    unsigned char typeFile; // Tipo de arquivo P2 ou P5
    fread(&typeFile, sizeof(unsigned char), 1, readFile);

    // Lendo as dimensões da imagem
    int width, height,max_gray;
    fread(&width, sizeof(int), 1, readFile);
    fread(&height, sizeof(int), 1, readFile);
    fread(&max_gray, sizeof(int), 1, readFile);

    // Criando uma struct para receber as informações
    PGMImage *img = allocatePGM(width, height, max_gray);

    // Lendo o arquivo e verificando seu tipo de acordo com a tradução em binaŕio
    if (typeFile == 0x02)
    {
        img->type[0] = 'P';
        img->type[1] = '2';
    }
    else if (typeFile == 0x05)
    {
        img->type[0] = 'P';
        img->type[1] = '5';
    }
    img->type[2] = '\0';

    // Como a imagem é quadrada, podemos passar tanto width ou height como argumento
    isHomogeneous(img, readFile, 0, 0, width);

    fclose(readFile);

    // Escreve o arquivo de saida
    createPGM(img, imgOutput);

    // Liberação de memória
    for (int i = 0; i < img->height; i++)
    {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}