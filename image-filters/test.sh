#! /bin/bash
PROG=image-filters

make clean
make MODE=do build
./$PROG.do -b original.bmp basic_blurred.bmp
objdump -S helpers.o > basic_exe.dump

make clean
make MODE=do ARCH=x86 build
./$PROG.do -b original.bmp x86_blurred.bmp 
objdump -S helpers.o > x86_exe.dump

