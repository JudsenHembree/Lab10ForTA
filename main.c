#include <stdio.h>
#include "functions.h"


typedef int** MATRIX;

int main(int argc, char* argv[]){
    header head;
    pixel** inputPPM;
    readHeader(&head,argv[1]);
    fprintf(stderr, "\n%s %d %d %d\n", head.magicNum, head.width, head.height, head.maxVal);
    inputPPM = readPPM(&head, argv[1]);

    pixel** copy = copyPPM(&head, inputPPM);
    pushPPM(&head, copy, "copied.ppm");
    

    #if threebythree == 1
        fprintf(stderr,"3x3 run\n");

        MATRIX kernal = allocateMatrix(3,3);
        kernal[0][0] = 0;
        kernal[0][1] = -1;  
        kernal[0][2] = 0;
        kernal[1][0] = -1;
        kernal[1][1] = 5;
        kernal[1][2] = -1;
        kernal[2][0] = 0;
        kernal[2][1] = -1;
        kernal[2][2] = 0; 

        showMatrix(3,3,kernal);
        filterPPM(&head, inputPPM, copy, kernal, 3);

        pushPPM(&head, copy, "sharpened3.ppm");

    #else
        fprintf(stderr,"5x5 run\n");
        MATRIX kernal5 = allocateMatrix(5,5);
        kernal5[0][0] = 0;kernal5[0][1] = 0;kernal5[0][2] = -1;kernal5[0][3] = 0;kernal5[0][4] = 0;
        kernal5[1][0] = 0;kernal5[1][1] = 0;kernal5[1][2] = -1;kernal5[1][3] = 0;kernal5[1][4] = 0;
        kernal5[2][0] = -1;kernal5[2][1] = -1;kernal5[2][2] = 9;kernal5[2][3] = -1;kernal5[2][4] = -1;
        kernal5[3][0] = 0;kernal5[3][1] = 0;kernal5[3][2] = -1;kernal5[3][3] = 0;kernal5[3][4] = 0;
        kernal5[4][0] = 0;kernal5[4][1] = 0;kernal5[4][2] = -1;kernal5[4][3] = 0;kernal5[4][4] = 0;
        showMatrix(5,5,kernal5);

        filterPPM(&head, inputPPM, copy, kernal5, 5);
        pushPPM(&head, copy, "sharpened5.ppm");

    #endif
    return 0;
}