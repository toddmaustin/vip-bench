#! /bin/bash
PROG=mnist-cnn

make clean
make MODE=do build
./$PROG.do -i mnist-cnn.kan mnist-test-x.knd > basic_out.dump
objdump -S $PROG.do > basic_exe.dump

make clean
make MODE=do ARCH=x86 build
./$PROG.do -i mnist-cnn.kan mnist-test-x.knd > x86_out.dump
objdump -S $PROG.do > x86_exe.dump

