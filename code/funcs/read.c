#include "../lib/prototypes.h"

// Função para pular comentários da nossa imagem
void jumpComments(FILE *file) {
    int ch; // Utilizando 'int' por causa do 'fgetc' que retorna inteiro. 
    while ((ch = fgetc(file)) == '#') { 
        while (fgetc(file) != '\n');
    }
    ungetc(ch, file); 
}

//Função para ler o arquivo PGM
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