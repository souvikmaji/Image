#include <stdio.h>
#include <math.h>

void dat2bmp(char *datname, char *bmpname, int* imgw, int* imgh)
{
    int bit_read, c, flag, current_bit = 0, count_height = 0, count_width = 0, w, h, width_backup;
    unsigned char bit_buffer;
    
    //************* COMPUTE HEIGHT AND WIDTH OF INPUT DAT FILE *************       
    FILE *Input_Dat_File = fopen(datname,"r");
    char ch = getc(Input_Dat_File);
    c = 0;  //Temp width counter
    flag = 0;
    while (ch != EOF)
    {
        if (ch == '\n')
        {   
            if(flag == 0)
                count_width = c;
            count_height += 1;
            flag = 1;
        }
        ch = getc(Input_Dat_File);
        c++;
    }
    
    //***************** HEIGHT,WIDTH COMPUTATION COMPLETE ******************
    
    //********** CREATING TEMP DAT FILE WITH WIDTH MULTIPLE OF 8 ***********
    
    width_backup = count_width;
    count_width -= count_width % 8;
    
    rewind(Input_Dat_File);
    FILE *Temp_Dat_File1 = fopen("TempDatFile1.dat","w");
    
    for(h=0; h<count_height; h++)
    {
        for(w=0; w<width_backup; w++)
        {                
            ch = fgetc(Input_Dat_File);
            if(count_width > w)
                fprintf(Temp_Dat_File1,"%d",ch-48);
        }
        ch = fgetc(Input_Dat_File);  //Discard the \n character
        fputc('\n',Temp_Dat_File1);
    }
    
    fclose(Input_Dat_File);
    fclose(Temp_Dat_File1);
    
    //****** TEMP DAT FILE WITH WIDTH MULTIPLE OF 8 CREATION COMPLETE ******
    
    //********** REVERSING LINES OF TEMP DAT FOR BMP FILE WRITING **********
    
    Temp_Dat_File1 = fopen("TempDatFile1.dat","r");
    FILE *Temp_Dat_File2 = fopen("TempDatFile2.dat","w");
    for(h=0; h<count_height; h++)
    {
        fseek( Temp_Dat_File1, -(count_width+2)*(h+1), SEEK_END );
        for(w=0; w<count_width; w++)
        {
            ch = getc(Temp_Dat_File1);
            if(ch=='1')
                fprintf(Temp_Dat_File2,"%d",1);
            else if(ch=='0')
                fprintf(Temp_Dat_File2,"%d",0);
        }
        ch = getc(Temp_Dat_File1);
        fprintf(Temp_Dat_File2,"\n");
    }
    fclose(Temp_Dat_File1);
    fclose(Temp_Dat_File2);        
    remove("TempDatFile1.dat");

    //************* TEMP DAT FILE READY FOR BMP FILE CREATION **************
    
    //********************* WRITE BMP FILE STRUCTURE ***********************
    
    static unsigned char color[1], /* 1 byte = 8 bits */
                         byte[1];
    unsigned short word[1]; /* 2 bytes */
    unsigned long  dword[1], /* 4 bytes */
                   bpp=1,
                   /* in bytes */
                   FileHeaderSize=14, 
                   InfoHeaderSize=40, // header_bytes
                   PaletteSize=pow(2,bpp)*4, // = 8 = number of bytes in palette
                   BytesPerRow,
                   FileSize,
                   OffBits,
                   BytesSize, // bytes in image portion
                   iByte, // number of byte
                   /* in pixels */
                   Height=count_height,
                   Width=count_width, 
                   /* in ... */
                   ix, iy, ix_,
                   bit=0, /* bit value */
                   bitNumber; /* bits are numberd from 0 to 7 */
    //-----------------------               
    BytesPerRow=(((Width * bpp)+31)/32)*4;
    BytesSize=BytesPerRow*Height;
    FileSize=FileHeaderSize+InfoHeaderSize+PaletteSize+BytesSize;
    OffBits=FileHeaderSize+InfoHeaderSize+PaletteSize;

    unsigned char bytes[BytesSize];
    //--------------------------      
    FILE *Output_BMP_File = fopen(bmpname, "wb"); /* b - binary mode */
    /* bmp file header */
    word[0]=19778;                                         fwrite(word,1,2,Output_BMP_File); /* file Type signature = BM */
    dword[0]=FileSize;                                     fwrite(dword,1,4,Output_BMP_File); /* FileSize */
    word[0]=0;                                             fwrite(word,1,2,Output_BMP_File); /* reserved1 */
    word[0]=0;                                             fwrite(word,1,2,Output_BMP_File); /* reserved2 */
    dword[0]=OffBits;                                      fwrite(dword,1,4,Output_BMP_File); /* OffBits */
    dword[0]=InfoHeaderSize;                               fwrite(dword,1,4,Output_BMP_File); 
    dword[0]=Width;                                        fwrite(dword,1,4,Output_BMP_File); 
    dword[0]=Height;                                       fwrite(dword,1,4,Output_BMP_File); 
    word[0]=1;                                             fwrite(word,1,2,Output_BMP_File); /* planes */
    word[0]=1;                                             fwrite(word,1,2,Output_BMP_File); /* Bits of color per pixel */
    dword[0]=0;                                            fwrite(dword,1,4,Output_BMP_File); /* compression type */
    dword[0]=0;                                            fwrite(dword,1,4,Output_BMP_File); /* Image Data Size, set to 0 when no compression */
    dword[0]=0;                                            fwrite(dword,1,4,Output_BMP_File); /*  */
    dword[0]=0;                                            fwrite(dword,1,4,Output_BMP_File); /*  */
    dword[0]=2;                                            fwrite(dword,1,4,Output_BMP_File); /*  number of used coloors*/
    dword[0]=0;                                            fwrite(dword,1,4,Output_BMP_File); /*  */
           
    /*  color table (palette) = 2 colors as a RGBA */
    /* color 0 = white */
    byte[0]=255;                                            fwrite(byte,1,1,Output_BMP_File); /* R */                                     
    byte[0]=255;                                            fwrite(byte,1,1,Output_BMP_File); /* G */    
    byte[0]=255;                                            fwrite(byte,1,1,Output_BMP_File); /* B */ 
    byte[0]=255;                                            fwrite(byte,1,1,Output_BMP_File); /*  A*/  
    /* color 1 = black */   
    byte[0]=0;                                              fwrite(byte,1,1,Output_BMP_File); /* R */                                     
    byte[0]=0;                                              fwrite(byte,1,1,Output_BMP_File); /*  G*/    
    byte[0]=0;                                              fwrite(byte,1,1,Output_BMP_File); /* B */    
    byte[0]=255;                                            fwrite(byte,1,1,Output_BMP_File); /* A */  
    
    //*********************** FILE STRUCTURE READY *************************
    
    for(c=0; c<BytesSize; c++)  //Clear all pixels (white)
        bytes[c]=0;
    
    Temp_Dat_File2 = fopen("TempDatFile2.dat","r");
    
    int index = 0;
    
    for(h=0; h<count_height; h++)
    {
        for(w=0; w<count_width/8; w++)
        {
            bit_buffer = 0;  //Clear bit buffer after 8 bit read
            for(c=0; c<8; c++)
            {
                ch = getc(Temp_Dat_File2);          
                bit_buffer = bit_buffer<<1;
                if(ch=='0')
                {    
                //printf("\n0C%c",ch);    
                    bit_buffer = bit_buffer|0;
                }
                else if(ch=='1')
                {
                //printf("\n1C%c",ch);    
                    bit_buffer = bit_buffer|1;
                }
            //printf("Bit Buffer = %d",bit_buffer);
            }
            bytes[index++] = bit_buffer;
        }
        ch = getc(Temp_Dat_File2);   //Discard the '\n' character
        index += (BytesSize/count_height - count_width/8);
    }
    
    //bytes[128] = 85;    //For Checking 
    
    fclose(Temp_Dat_File2);        
    
    fwrite(bytes,1,BytesSize,Output_BMP_File);  
    fclose(Output_BMP_File);
    remove("TempDatFile2.dat");
    
    *imgw = count_width; *imgh = count_height;
}
 
 
