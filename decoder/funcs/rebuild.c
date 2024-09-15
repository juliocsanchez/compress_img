#include "../lib/directives.h"

// Função para alocar espaço na memória dinamicamente
PGMImage *allocatePGM(int width, int height, int max_gray)
{
    PGMImage *img = (PGMImage *)malloc(sizeof(PGMImage));
    img->width = width;
    img->height = height;
    img->max_gray = max_gray;

    // Alocação de espaço para receber os valores de cada píxel
    img->pixels = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++)
    {
        img->pixels[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }

    // Retorna a imagem com os espaços a serem preenchidos pelos quadrantes do bitstream
    return img;
}

// Função para preencher o cabeçalho
void createPGM(PGMImage *img, const char *filename)
{

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Verifica a possibilidade do arquivo ser P2, e caso seja, escreve o cabeçalho do arquivo
    if (img->type[1] == '2')
    {
        fprintf(file, "P2\n%d %d\n%d\n", img->width, img->height, img->max_gray);
        for (int i = 0; i < img->height; i++)
        {
            for (int j = 0; j < img->width; j++)
            {
                fprintf(file, "%d ", img->pixels[i][j]);
            }
            fprintf(file, "\n");
        }
        // Verifica a possibilidade do arquivo ser P5, e caso seja, escreve o cabeçalho do arquivo
    }
    else if (img->type[1] == '5')
    {
        fprintf(file, "P5\n%d %d\n%d\n", img->width, img->height, img->max_gray);
        for (int i = 0; i < img->height; i++)
        {
            fwrite(img->pixels[i], sizeof(unsigned char), img->width, file); // o 'fwrite' trata especificamente dados binários, como os do arquivo P5
        }
    }

    fclose(file);
}

// Função para preencher o quadrante com a cor especificada no bitstream
void toFill(PGMImage *img, int x, int y, int size, unsigned char value)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            img->pixels[y + i][x + j] = value;
        }
    }
}