
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=fft-int
OBJS=fft-int.o mathlib.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO fft-int.out
	rm -f FOO

