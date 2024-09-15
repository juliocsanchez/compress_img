typedef struct {
    char type[3];
    int width;
    int height;
    int max_gray;
    unsigned char **pixels;
} PGMImage;