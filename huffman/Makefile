CFLAGS=-O2 -Wall -Werror
ARCH=i386
CFLAGS=-g -Wall -Werror -arch ${ARCH}
LDFLAGS=-arch ${ARCH}

all: huffcode libhuffman.a

huffcode: huffcode.o libhuffman.a
	$(CC) $(LDFLAGS) -o $@ huffcode.o libhuffman.a

huffman.o: huffman.h

libhuffman.a: huffman.o
	$(AR) r $@ $<

clean:
	$(RM) *.o *~ core huffcode huffcode.exe libhuffman.a
