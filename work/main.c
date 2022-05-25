#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int i, j;

void preparation(unsigned char *od, int ih, int iw){
    for(i=2;i<ih-1;i++)
        for(j=2;j<iw-1;j++){
            if(od[iw*i+j]<100)
                od[iw*i+j]=0;
            if(od[iw*i+j]>160)
                od[iw*i+j]=255;
        }
    return;
}

void fSobel(unsigned char *od, unsigned char *dc, int ih, int iw){
    unsigned char x, y;
    for(i=2;i<ih-1;i++)
        for(j=2;j<iw-1;j++){
            x=(-1)*od[iw*(i-1)+(j-1)]-2*od[iw*i+(j-1)]-1*od[iw*(i+1)+(j-1)]+
                +1*od[iw*(i-1)+(j+1)]+2*od[iw*i+(j+1)]+1*od[iw*(i+1)+(j+1)];
            y=(-1)*od[iw*(i-1)+(j-1)]-2*od[iw*(i-1)+j]-1*od[iw*(i-1)+(j+1)]+
                +1*od[iw*(i+1)+(j-1)]+2*od[iw*(i+1)+j]+1*od[iw*(i+1)+(j+1)];
            dc[iw*i+j]=sqrt(x*x+y*y);
        }
    return;
}

void fGauss(unsigned char *od, unsigned char *dc, int ih, int iw){
     for(i=2;i<ih-1;i++)
        for(j=2;j<iw-1;j++){
            dc[iw*i+j]=0.12*od[iw*i+j]+0.12*od[iw*(i+1)+j]+0.12*od[iw*(i-1)+j];
            dc[iw*i+j]=dc[iw*i+j]+0.12*od[iw*i+(j+1)]+0.12*od[iw*i+(j-1)];
            dc[iw*i+j]=dc[iw*i+j]+0.09*od[iw*(i+1)+(j+1)]+0.09*od[iw*(i+1)+(j-1)];
            dc[iw*i+j]=dc[iw*i+j]+0.09*od[iw*(i-1)+(j+1)]+0.09*od[iw*(i-1)+(j-1)];
        }
   return;
}

void colouring(unsigned char *dc, unsigned char *mcod, int ih, int iw, int n){
    int c[iw*ih];
    for(i=0;i<iw*ih;i++)
        c[i]=dc[i];
    for(i=1;i<iw*ih;i++) {
        mcod[i*n]=80+c[i]+0.5*c[i-1];
        mcod[i*n+1]=45+c[i];
        mcod[i*n+2]=150+c[i];
        mcod[i*n+3]=255;
    }
    return;
}

int main(void){
    char *inputPath = "hampster.png";
    //char *inputPath = "teeth.png";
    int iw, ih, n; //ширина, высота и количество цветовых каналов
    int k=0;

    unsigned char *idata=stbi_load(inputPath, &iw, &ih, &n, 0);
    if(idata==NULL){
        printf("ERROR: can't read file %s\n", inputPath);
        return 1;
    }

    unsigned char *odata=(unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    unsigned char *datacopy=(unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    unsigned char *mcodata=(unsigned char*)malloc(ih*iw*n*sizeof(unsigned char));

    for(i=0;i<ih*iw*n;i=i+n){
         odata[k]=0.299*idata[i]+0.587*idata[i+1]+0.114*idata[i+2];
         k++;
    }

    preparation(odata, ih, iw);
    //fSobel(odata, datacopy, ih, iw);
    fGauss(odata, datacopy, ih, iw);
    colouring(datacopy, mcodata, ih, iw, n);

    char *outputPath="output.png";

    //stbi_write_png(outputPath, iw, ih, 1, datacopy, 0);
    stbi_write_png(outputPath, iw, ih, n, mcodata, 0);
    stbi_image_free(idata);
    stbi_image_free(odata);
    stbi_image_free(datacopy);
    stbi_image_free(mcodata);
    return 0;
}
