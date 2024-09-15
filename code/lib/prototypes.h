#include "directives.h"

void jumpComments(FILE *);
PGMImage *loadPGM(const char *);
int isHomogeneous(PGMImage *, int, int, int, int);
unsigned char grayTone(PGMImage *, int, int, int);
void bitstreamGenerate(PGMImage *, FILE *, int, int, int, int);
void pgmToFilebin(PGMImage *, const char *, int);
void freePGM(PGMImage *);