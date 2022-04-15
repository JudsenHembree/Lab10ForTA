#include "functions.h"

pixel** allocatePPM(header* head){
    pixel** ppm = (pixel**)malloc(sizeof(pixel*)* head->width);
    for(int i = 0; i<head->width; i++){
        ppm[i]=(pixel*)malloc(sizeof(pixel)*head->height);
    }
    return ppm;
}

int** allocateMatrix(int width, int height){
    int** matrix = (int**)malloc(sizeof(int*)* width);
    for(int i = 0; i<width; i++){
        matrix[i]=(int*)malloc(sizeof(int)*height);
    }
    return matrix;
}

void showMatrix(int width, int height, int** matrix){
    for(int i = 0; i<width; i++){
        for(int j = 0; j<height; j++){
            fprintf(stderr, "%d ", matrix[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

void pushPPM(header* head, pixel** ppm, char* out){
    FILE* outfile = fopen(out,"w");
    fprintf(outfile,"%s\n%d %d\n%d\n", head->magicNum, head->width, head->height, head->maxVal);
    for(int width = 0; width<head->width; width++){
        for(int height = 0; height<head->height; height++){
            fprintf(outfile,"%c",ppm[width][height].red);
            fprintf(outfile,"%c",ppm[width][height].green);
            fprintf(outfile,"%c",ppm[width][height].blue);

            //fprintf(stderr, "\n%d %d %d\n", ppm[width][height].red, ppm[width][height].green, ppm[width][height].blue);
        }
    }

}

pixel** readPPM(header* head, char* file){
    pixel** ppm = allocatePPM(head);
    FILE* openedFile = fopen(file, "r");
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    skipHeader(openedFile);
    for(int width = 0; width<head->width; width++){
        for(int height = 0; height<head->height; height++){
            fscanf(openedFile, "%c", &red);
            fscanf(openedFile, "%c", &green);
            fscanf(openedFile, "%c", &blue);
            ppm[width][height].red = (int)red;
            ppm[width][height].green = (int)green;
            ppm[width][height].blue = (int)blue;
        }
    }
    //pushPPM(head, ppm, "out.ppm");
    fclose(openedFile);
    return ppm;
}

pixel** copyPPM(header* head, pixel** ppm){
    pixel** copy = allocatePPM(head); 
    for(int width = 0; width<head->width; width++){
       memcpy(copy[width],ppm[width],sizeof(pixel)*head->height); 
    }
    return copy;
}

void skipHeader(FILE* file){

    char temp;
    int tempInt;
    uint headerComplete = 0;
    uint magicNumSet = 0;
    uint widthSet = 0;
    uint heightSet = 0;
    uint maxValSet = 0;

    while (fscanf(file, "%c", &temp)){
        if(temp != '#' && temp != '\n' && temp != ' '){
            if(!magicNumSet){
                //head->magicNum[0] = temp;
                fscanf(file, "%c", &temp);
                //head->magicNum[1] = temp;
                magicNumSet = 1;
            }else if(!widthSet){
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%d", &tempInt);
                //head->width = tempInt;
                widthSet = 1;
            }else if(!heightSet){
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%d", &tempInt);
                //head->height = tempInt;
                heightSet = 1;
            }else if(!maxValSet){
                fseek(file, -1, SEEK_CUR);
                fscanf(file, "%d\n", &tempInt);
                //head->maxVal = tempInt;
                return;
            }
        }else if(temp == '#'){
            lookForEndline(file);
        }
    }
}

void readHeader(header* head, char* file){

    FILE* working = fopen(file,"r");
    unsigned char temp;
    int tempInt;
    uint headerComplete = 0;
    uint magicNumSet = 0;
    uint widthSet = 0;
    uint heightSet = 0;
    uint maxValSet = 0;

    while (fscanf(working, "%c", &temp)){
        if(temp != '#' && temp != '\n' && temp != ' '){
            if(!magicNumSet){
                head->magicNum[0] = temp;
                fscanf(working, "%c", &temp);
                head->magicNum[1] = temp;
                magicNumSet = 1;
            }else if(!widthSet){
                fseek(working, -1, SEEK_CUR);
                fscanf(working, "%d", &tempInt);
                head->width = tempInt;
                widthSet = 1;
            }else if(!heightSet){
                fseek(working, -1, SEEK_CUR);
                fscanf(working, "%d", &tempInt);
                head->height = tempInt;
                heightSet = 1;
            }else if(!maxValSet){
                fseek(working, -1, SEEK_CUR);
                fscanf(working, "%d", &tempInt);
                head->maxVal = tempInt;
                return;
            }
        }else if(temp == '#'){
            lookForEndline(working);
        }
    }
    fclose(working);
}


void lookForEndline(FILE* file){
    char temp;
    while(fscanf(file, "%c", &temp)){
        if (temp == '\n'){
            return; 
        }
    }
    fprintf(stderr,"Failing at lookForEndline.\n");
    exit(EXIT_FAILURE);
}



void filterPPM(header* head, pixel** ppm, pixel** copy, int** filter, int ksize)
{
    int untouch = ksize/2;
    fprintf(stderr, "\n%d\n", untouch);
    int i, j;
    for(i = untouch; i < head->width-(untouch); i++)
    {
        for(j = untouch; j < head->height-(untouch); j++)
        {
            copy[i][j] = filterPixel(ppm, filter, i, j, untouch);
            //fprintf(stderr,"\n%d %d %d\n", copy[i][j].red, copy[i][j].green, copy[i][j].blue);
        }
    }
}

pixel filterPixel(pixel** ppm, int** filter, int width, int height, int ksize){
    int valRed = 0;
    int valGreen = 0;
    int valBlue = 0;
    int matrixI = 0;
    int matrixJ = 0;
    int end = ksize+1;
    int filterEnd = 2*ksize;
    //should be -1 && < 2 for 3x3
    //should be -2 && < 3 for 5x5
    for(int i=-ksize;i<end;i++){
        for(int j=-ksize;j<end;j++){
            valRed += ppm[width + i][height + j].red * filter[matrixI][matrixJ];
            valGreen += ppm[width + i][height + j].green * filter[matrixI][matrixJ];
            valBlue += ppm[width + i][height + j].blue * filter[matrixI][matrixJ];
            if(matrixJ<filterEnd){
                matrixJ++;
            }else{
                matrixJ=0;
                matrixI++;
            }
        }
    }
    pixel ret = {clamp(valRed), clamp(valGreen), clamp(valBlue)};
    //fprintf(stderr, "\n%d %d %d\n", ret.red, ret.green, ret.blue);
    return ret;
}

int clamp(int item){
    if(item<0){
        return 0;
    }else if(item>255){
        return 255;
    }
    return item;
}