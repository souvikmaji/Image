#include<stdio.h>
#include"BMP2DAT.h"

int main()
{
	char imageFile[] = "Input.bmp";
	char dataFile[] = "Data";
	int height, width;
	
	bmp2dat(imageFile, dataFile, &height, &width);
	return 0;
}
