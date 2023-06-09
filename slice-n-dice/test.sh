#! /bin/bash
PROG=slice-n-dice

make clean
make MODE=do build
./$PROG.do basic.log 200000 > basic_out.dump
objdump -S $PROG.do > basic_exe.dump

make clean
make MODE=do ARCH=x86 build
./$PROG.do x86.log 200000 > x86_out.dump
objdump -S $PROG.do > x86_exe.dump

