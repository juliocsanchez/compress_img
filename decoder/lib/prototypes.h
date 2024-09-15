#include "directives.h"

PGMImage *allocatePGM(int, int, int);

void createPGM(PGMImage *, const char *);

void toFill(PGMImage *, int, int, int, unsigned char);

void isHomogeneous(PGMImage *, FILE *, int, int, int);

void decodeBitstream(const char *, const char *);