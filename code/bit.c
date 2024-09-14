#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_GRAY 255


typedef struct {
    int width;
    int height;
    int max_gray;
    unsigned char **pixels;
} PGMImage;


void skipComments(FILE *file) {
    int ch;
    while ((ch = fgetc(file)) == '#') { 
        while (fgetc(file) != '\n');
    }
    ungetc(ch, file); 
}


PGMImage* loadPGM(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    PGMImage *img = (PGMImage *)malloc(sizeof(PGMImage));
    
   
    char pgmType[3];
    skipComments(file);
    fscanf(file, "%2s\n", pgmType);
    
    if (pgmType[0] != 'P' || (pgmType[1] != '2' && pgmType[1] != '5')) {
        printf("Formato PGM inválido. Apenas P2 e P5 são suportados.\n");
        fclose(file);
        free(img);
        return NULL;
    }

    skipComments(file);
    fscanf(file, "%d %d\n", &img->width, &img->height);
    
    skipComments(file);
    fscanf(file, "%d\n", &img->max_gray);

    img->pixels = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }

    if (pgmType[1] == '5') {
        
        for (int i = 0; i < img->height; i++) {
            fread(img->pixels[i], sizeof(unsigned char), img->width, file);
        }
    } else if (pgmType[1] == '2') {
       
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


int isHomogeneous(PGMImage *img, int x, int y, int size, int tolerance) {
    if (x + size > img->width || y + size > img->height) {
        return 0;  
    }
    unsigned char first_pixel = img->pixels[y][x];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (y + i >= img->height || x + j >= img->width) continue; 
            if (abs(img->pixels[y + i][x + j] - first_pixel) > tolerance) {
                return 0;
            }
        }
    }
    return 1; 
}


unsigned char averageGray(PGMImage *img, int x, int y, int size) {
    int total = 0, count = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (y + i < img->height && x + j < img->width) {
                total += img->pixels[y + i][x + j];
                count++;
            }
        }
    }
    return total / count;
}


void quadtreeCompressToBitstream(PGMImage *img, FILE *bitstream, int x, int y, int size, int tolerance) {
    if (isHomogeneous(img, x, y, size, tolerance)) {

        fputc(1, bitstream);
        unsigned char avg_gray = averageGray(img, x, y, size);
        fputc(avg_gray, bitstream);
    } else {
     
        fputc(0, bitstream);
        int half = size / 2;

        if (half > 0) {
            quadtreeCompressToBitstream(img, bitstream, x, y, half, tolerance);              // Quadrante superior esquerdo
            quadtreeCompressToBitstream(img, bitstream, x + half, y, half, tolerance);       // Quadrante superior direito
            quadtreeCompressToBitstream(img, bitstream, x, y + half, half, tolerance);       // Quadrante inferior esquerdo
            quadtreeCompressToBitstream(img, bitstream, x + half, y + half, half, tolerance); // Quadrante inferior direito
        } else {
    
            fputc(1, bitstream);
            fputc(img->pixels[y][x], bitstream);
        }
    }
}


void compressPGMToBitstream(PGMImage *img, const char *output_filename, int tolerance) {
    FILE *bitstream = fopen(output_filename, "wb");
    if (!bitstream) {
        printf("Erro ao criar o arquivo de bitstream.\n");
        return;
    }

    fwrite(&img->width, sizeof(int), 1, bitstream);
    fwrite(&img->height, sizeof(int), 1, bitstream);

    int size = img->width > img->height ? img->width : img->height; 
    quadtreeCompressToBitstream(img, bitstream, 0, 0, size, tolerance);

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

    compressPGMToBitstream(img, argv[2], tolerance);

    freePGM(img);

    return 0;
}
