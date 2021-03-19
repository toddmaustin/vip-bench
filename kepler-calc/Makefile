

LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=kepler-calc
OBJS=kepler-calc.o mathlib.o

include ../Makefile

test:
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO kelp-calc.out
	rm -f FOO

