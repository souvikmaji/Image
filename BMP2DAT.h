#include <stdio.h>
#include <malloc.h>

void bmp2dat(char *bmpname, char *datname, int* imgw, int* imgh)
{
    unsigned char head[54];
    FILE *f = fopen(bmpname,"rb");

    //Read 54 bytes BMP Header
    fread(head, 1, 54, f);

    int w = head[18] + ( ((int)head[19]) << 8) + ( ((int)head[20]) << 16) + ( ((int)head[21]) << 24);
    int h = head[22] + ( ((int)head[23]) << 8) + ( ((int)head[24]) << 16) + ( ((int)head[25]) << 24);

    //Lines are aligned on 4-byte boundary
    int lineSize = (w / 32) * 4;
    if( w % 32 )
        lineSize += 4;
    int fileSize = lineSize * h;

    unsigned char *img = malloc(w * h), *data = malloc(fileSize);

    //Skip the Header
    fseek(f,54,SEEK_SET);

    //Skip the Palette - 2 RGB quads, 8 bytes
    fseek(f, 8, SEEK_CUR);

    //Read the Data
    fread(data,1,fileSize,f);

    //Decode Bits
    int i, j, k, rev_j;
    for(j = 0, rev_j = h - 1; j < h ; j++, rev_j--)
    {
      for(i = 0 ; i <= w / 8; i++)
      {
         int fpos = j * lineSize + i, pos = rev_j * w + i * 8;
         for(k = 0 ; k < 8 ; k++)
            if( i < w / 8  ||  k >= 8 - (w % 8) )
               img[pos + (7 - k)] = (data[fpos] >> k ) & 1;
      }
    }

    free(data);
    
    *imgw = w; *imgh = h;
    
    FILE *fp = fopen(datname,"w");
    
    for(j = 0 ; j < h ; j++)
    {
        for(i = 0 ; i < w ; i++)
            fprintf(fp,"%c", w%8?(img[j * w + i] ? '0' : '1'):(img[j * w + i] ? '1' : '0') );

        fprintf(fp,"\n");
    }
    
    fclose(fp);
    
    free(img);
}

