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

//matrix utils
int** allocateMatrix(int, int);
void showMatrix(int, int, int**);

//reading utils
void readHeader(header*, char*);
void skipHeader(FILE*);
pixel** readPPM(header*, char*);
void lookForEndline(FILE*);

//full ppm utils
pixel** allocatePPM(header*);
void pushPPM(header*, pixel**, char*);
pixel** copyPPM(header*, pixel**);

//calcs
void filterPPM(header*, pixel**, pixel**, int**, int);
pixel filterPixel(pixel**, int**, int, int, int);
int clamp(int);
#endif