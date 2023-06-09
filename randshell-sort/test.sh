#! /bin/bash
PROG=randshell-sort

make clean
make MODE=do build
./$PROG.do > basic_out.dump
objdump -S $PROG.do > basic_exe.dump

make clean
make MODE=do ARCH=x86 build
./$PROG.do > x86_out.dump
objdump -S $PROG.do > x86_exe.dump

