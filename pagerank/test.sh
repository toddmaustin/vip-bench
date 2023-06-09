#! /bin/bash
PROG=pagerank

make clean
make MODE=do build
./$PROG.do --filename r1000a.dimacs --fmt DIMACS --style pull --df 0.85 > basic_out.dump
objdump -S $PROG.do > basic_exe.dump

make clean
make MODE=do ARCH=x86 build
./$PROG.do --filename r1000a.dimacs --fmt DIMACS --style pull --df 0.85 > x86_out.dump
objdump -S $PROG.do > x86_exe.dump

