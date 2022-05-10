#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(void){
    // строка, путь к файлу
    char*inputPath = "hampster.png";
    //char*inputPath = "teeth.png";
    int iw, ih, n; //ширина, высота и количество цветовых каналов

    //Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
   //printf("n=%d",n);
    if(idata==NULL){
        printf("ERROR: can't read file %s\n", inputPath);
        return 1;
    }

    int i, j, k=0;
    unsigned char* odata = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));

    for(i=0;i<ih*iw*n;i=i+n){
         odata[k]=0.299*idata[i]+0.587*idata[i+1]+0.114*idata[i+2];
         k++;
    }
    for(i=2;i<ih-1;i++)
        for(j=2;j<iw-1;j++)
            odata[iw*i+j]*=3;

    char* outputPath="output.png";
    int one=1, zero=0;

    stbi_write_png(outputPath, iw, ih, one, odata, zero);
    //stbi_image_write(outputPath, iw, ih, 2, MyImage, 0);
    //printf("Изображение размера %d в высоту и %d в ширину с количеством каналов %d считано", ih, iw, n);
    stbi_image_free(idata);
    stbi_image_free(MyImage);
    stbi_image_free(odata);
    return 0;
}
