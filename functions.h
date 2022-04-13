#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNCS_H
#define FUNCS_H

typedef struct header_t{
    int height;
    int width;
    int maxVal;
    char magicNum[2];
}header;

typedef struct pixel_t{
    int red;
    int green;
    int blue;
}pixel;

void readHeader(header*, char*);
pixel** readPPM(header*, char*);
void lookForEndline(FILE*);
void pushPPM(header*, pixel**, char*);
pixel** allocatePPM(header*);
void skipHeader(FILE*);
pixel** copyPPM(header*, pixel**);

#endif