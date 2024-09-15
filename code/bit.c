#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    char type[3];
    int width;
    int height;
    int max_gray; // Valor máximo da variação de cinza 
    unsigned char **pixels;
} PGMImage;

// Função para pular comentários da nossa imagem
void jumpComments(FILE *file) {
    int ch; // Utilizando 'int' por causa do 'fgetc' que retorna inteiro. 
    while ((ch = fgetc(file)) == '#') { 
        while (fgetc(file) != '\n');
    }
    ungetc(ch, file); 
}


PGMImage* loadPGM(const char *filename) {

    FILE *file = fopen(filename, "rb");// O ponteiro '*file' agora recebe o arquivo aberto

    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    PGMImage *img = (PGMImage *)malloc(sizeof(PGMImage)); //Ponteiro para a nossa struct PGMImage
    
    jumpComments(file);
    fscanf(file, "%2s\n", img->type); //Armazena o tipo do arquivo
    
    //Verificação se o arquivo é P2 ou P5
    if (img->type[0] != 'P' || (img->type[1] != '2' && img->type[1] != '5')) {
        printf("Formato PGM inválido. Apenas P2 e P5 são suportados.\n");
        fclose(file);
        free(img);
        return NULL;
    }


    jumpComments(file); // Pular possivéis comentários ao lado do cabeçalho
    fscanf(file, "%d %d\n", &img->width, &img->height); // Armazenar altura e largura
    
    jumpComments(file); // Pular possíveis comentários após da altura e largura
    fscanf(file, "%d\n", &img->max_gray);// Ler máxima luminecência


    //Alocar espaço para o ponteiro contido na struct PGMImage
    img->pixels = (unsigned char **)malloc(img->height * sizeof(unsigned char *)); 
    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }

    // O programa visa alterar a maneira de ler o arquivo de acordo com o tipo da mesma(Binária ou texto)
    if (img->type[1] == '5') {
        for (int i = 0; i < img->height; i++) {
            fread(img->pixels[i], sizeof(unsigned char), img->width, file);//Lê o pixel de cada linha 
        }
    } else if (img->type[1] == '2') {
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                int pixel;
                fscanf(file, "%d", &pixel);
                img->pixels[i][j] = (unsigned char)pixel;
            }
        }
    }

    fclose(file);
    return img;
}


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
