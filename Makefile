CC=gcc
CFLAGS=-lm -g
LDFLAGS=
SOURCES=BMP2DAT.c DAT2BMP.c BMPcutter.c itoa.c
OBJ=$(SOURCES:.c = .o)
EXECUTABLE=BMPcutter

all:$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o *~ BMPcutter *.bmp 
