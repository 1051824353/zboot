topdir = .
include $(topdir)/MCONFIG

DISK = /dev/sdb
SUBDIRS = mbr lib init

all:
	set -e ; for i in $(SUBDIRS) ; do $(MAKE) -C $$i $@ ; done

install:
	$(DD) bs=512 conv=notrunc count=1 if=./base of=$(DISK)
	$(DD) bs=440 conv=notrunc count=1 if=./mbr/mbr of=$(DISK)
	mkfs.vfat $(DISK)1
	mount $(DISK)1 /tmp/syslinux.mnt.3361.0/
	cp ./init/init /tmp/syslinux.mnt.3361.0/
	mkdir /tmp/syslinux.mnt.3361.0/aaa
	mkdir /tmp/syslinux.mnt.3361.0/aaa/ccc
	touch /tmp/syslinux.mnt.3361.0/aaa/ccc/ddd
	touch /tmp/syslinux.mnt.3361.0/aaa/bbb
	umount /tmp/syslinux.mnt.3361.0/

cl:
	set -e ; for i in $(SUBDIRS) ; do $(MAKE) -C $$i $@ ; done

qe:
	qemu -hda $(DISK) -k en-us
qede:
	qemu -hda $(DISK) -gdb tcp::1234 -S -k en-us

