topdir = ..
include $(topdir)/MCONFIG

BIN = init
all: $(BIN)

INCLUDE = -I../include

LIB = ../lib/libz.a

COBJ = cstart.o ls.o cd.o pwd.o iso.o
AOBJ = init.o

init: init.elf checksize.pl
	$(OBJCOPY) -O binary $< $@

init.elf: $(COBJ) $(AOBJ)
	$(LD) $(LDFLAGS) -T init.ld -e _start -o $@ $(COBJ) $(AOBJ) $(LIB)

%.o:%.c
	$(CC) $(CFLAG) -c $< $(INCLUDE)
%.o:%.s
	$(CC) $(SFLAGS) -Wa,-a=$*.lst -c -o $@ $<

cl:
	rm -f *.o *.elf *.lst .*.d $(BIN)
