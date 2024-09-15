#include "../lib/prototypes.h"

// Função para verificar se o quadrante é homogêneo
int isHomogeneous(PGMImage *img, int x, int y, int size, int tolerance) {
    unsigned char first_pixel = img->pixels[y][x]; // Armazena o primeiro pixel do quadrante
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (abs(img->pixels[y + i][x + j] - first_pixel) > tolerance) { //Método do Max e mínimo
                return 0; //Não homogênea
            }
        }
    }
    return 1; // Homogênea
}

//Função para saber o tom de cinza daquele quadrante
unsigned char grayTone(PGMImage *img, int x, int y, int size) {
    int total = 0, count = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (y + i < img->height && x + j < img->width) {
                // Calcula a soma dos valores de cada pixel naquela região em específica 
                total += img->pixels[y + i][x + j]; 
                //Calcula quantos pixels tem naquela região
                count++;
            }
        }
    }
    return total / count; // Retorna a média do tom de cinza daquela região
}

//Função para gerar bitstream
void bitstreamGenerate(PGMImage *img, FILE *output, int x, int y, int size, int tolerance) {

    if (isHomogeneous(img, x, y, size, tolerance)) {

        fputc(1, output);// Se for homogêneo, escreve 1 no arquivo de saída
        unsigned char tone = grayTone(img, x, y, size);// Calcula o tom de cinza do quadrante
        fputc(tone, output);//Escreve o tom no arquivo de saída

    } else {
     
        fputc(0, output); //Indica que vai ser dividido para o arquivo de sáida
        int half = size / 2; // A cada quadrante 'filho' ele percorre a metade do tamanho do quadrante 'pai'

        // Verifica se ainda há possibilidade de divisão do respectivo quadrante
        if (half > 0) {
            bitstreamGenerate(img, output, x, y, half, tolerance);// Quadrante superior esquerdo
            bitstreamGenerate(img, output, x + half, y, half, tolerance); // Quadrante superior direito
            bitstreamGenerate(img, output, x, y + half, half, tolerance); // Quadrante inferior esquerdo
            bitstreamGenerate(img, output, x + half, y + half, half, tolerance); // Quadrante inferior direito
        } else {
            fputc(1, output);
            fputc(img->pixels[y][x], output);
        }
    }
}
