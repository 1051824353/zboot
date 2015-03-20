topdir = ..
include $(topdir)/MCONFIG


INCLUDE = -I../include

BIN = libz.a

COBJ = print.o string.o common.o dlmalloc.o fat.o
AOBJ = bios.o

all:
	make cl;make $(BIN)

$(BIN):$(AOBJ) $(COBJ)
	$(AR) rs $@ $(AOBJ) $(COBJ)

%.o:%.c
	$(CC) $(CFLAG) -c $< $(INCLUDE)
%.o:%.s
	$(CC) $(SFLAG) -Wa,-a=$*.lst -c  $<

cl:
	rm -rf *.o $(BIN) 
