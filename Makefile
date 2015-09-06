CC=gcc
CFLAGS=-lmg
DEPS=BMP2DAT.h DAT2BMP.h
OBJ=BMPcutter.c

%.o:%.c $(DEPS)
	$(CC) -c -o $@ $<$(CFLAGS)
BMPcutter:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ BMPcutter *.bmp 
