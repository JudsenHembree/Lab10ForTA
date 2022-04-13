#include <stdio.h>
#include "functions.h"

int main(int argc, char* argv[]){
    //header* head = malloc(sizeof(header));
    header head;
    pixel** inputPPM;
    readHeader(&head,argv[1]);
    fprintf(stderr, "\n%s %d %d %d\n", head.magicNum, head.width, head.height, head.maxVal);
    inputPPM = readPPM(&head, argv[1]);

    pixel** copy = copyPPM(&head, inputPPM);
    pushPPM(&head, copy, "copied.ppm");
    return 0;
}