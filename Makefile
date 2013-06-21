# To build with a specific architecture, set the ARCH flag on the make command line like this:
# make ARCH=i386
# or
# make ARCH=x86_64

CFLAGS=-g -Wall -Werror

ifdef ARCH
LDFLAGS+=-arch ${ARCH}
CFLAGS+=-arch ${ARCH}
endif

all: huffcode libhuffman.a

huffcode: huffcode.o libhuffman.a
	$(CC) $(LDFLAGS) -o $@ huffcode.o libhuffman.a

huffman.o: huffman.h

libhuffman.a: huffman.o
	$(AR) r $@ $<

clean:
	$(RM) *.o *~ core huffcode huffcode.exe libhuffman.a
