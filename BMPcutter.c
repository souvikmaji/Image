//Program for cropping Monochrome 24-bit BMP files using predefined coordinates

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "BMP2DAT.h"
#include "DAT2BMP.h"
#include "itoa.c"
int mkdir(const char *pathname, mode_t mode);

int main( int argc , char** argv )
{    
    int width, height, i, j, l, TopLeftX, TopLeftY, BottomRightX, BottomRightY, Folder, lines = 0, tw, th;
    char imgname[30];   //To hold cropped images name
    char imgpath[30];   //To hold full path of the image files
    
    if( argc != 3 )
    {
        printf("\nUsage:- BMPcutter <Input_BMP_Filename>.bmp <Co-ordinate_Filename>.dat\n\nNote-\n     (1)Each line in Co-ordinate file must be of following format:\n         TopLeftX TopLeftY BottomRightX BottomRightY <Folder_name_as_number>\n\n     (2)The BMP File must be a 24-bit Windows Monochrome BMP file...\n\n");
        return(0);
    }
        
    bmp2dat(argv[1], "BinarizedDatFile.dat", &width, &height);
    //dat2bmp("BinarizedDatFile.dat", "Output.bmp", &width, &height);
    
    FILE *cfile = fopen(argv[2], "r");
    FILE *dfile = fopen("BinarizedDatFile.dat", "r");
    FILE *tfile;
    
    //***********Count no. of lines in Coordinate File***********//
    while(!feof(cfile))
    {
      char ch = fgetc(cfile);
      if(ch == '\n')
      {
        lines++;
      }
    }
    rewind(cfile);
    //****************Counting complete*****************//
    
    for(l=0; l<lines; l++)
    {        
        tfile = fopen("Temp.dat", "w");
        fscanf(cfile, "%d%d%d%d%d", &TopLeftX, &TopLeftY, &BottomRightX, &BottomRightY, &Folder);
        itoa(Folder, imgpath, 10);
        mkdir(imgpath,ACCESSPERMS);
        for(i=TopLeftY; i<BottomRightY; i++)
        {
            fseek(dfile, (((width + 2) * i) + TopLeftX), SEEK_SET);
            for(j=TopLeftX; j<BottomRightX; j++)
            {
                fprintf(tfile, "%d", fgetc(dfile)-48);
            }
            fprintf(tfile, "\n");
        }
        fclose(tfile);
        itoa(l, imgname, 10);
        strcat(imgpath, "\\");
        strcat(imgname, ".bmp");
        strcat(imgpath,imgname);
        dat2bmp("Temp.dat", imgpath, &tw, &th);
    }
    
    fclose(cfile);
    fclose(dfile);
    remove("Temp.dat");
        
    return(0);
}
