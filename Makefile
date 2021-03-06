
all: circle vterm
	cd src && make

clean:
	cd src && make clean
	rm -rf build/*
	rm -f libvterm/src/encoding/*.inc

vterm:
	cd libvterm/src && make -f ../../Makefile.libvterm

install: all
	cp src/kernel.img /Volumes/PIVT
	diskutil umountdisk PIVT

circle: circle-stdlib/Config.mk

circle-stdlib/Config.mk:
	cd circle-stdlib && ./configure && make

mrproper: clean
	cd circle-stdlib && make mrproper

