topdir = ..
include $(topdir)/MCONFIG

all: mbr

mbr: mbr.elf checksize.pl
	$(OBJCOPY) -O binary $< $@

mbr.elf: mbr.o
	$(LD) $(LDFLAGS) -T mbr.ld -e _start -o $@ mbr.o

%.o:%.s
	$(CC) $(SFLAGS) -Wa,-a=$*.lst -c -o $@ $<

cl:
	rm -f *.o *.elf *.lst .*.d mbr
