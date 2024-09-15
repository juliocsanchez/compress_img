typedef struct {
    char type[3];
    int width;
    int height;
    int max_gray; // Valor máximo da variação de cinza 
    unsigned char **pixels;
} PGMImage;
