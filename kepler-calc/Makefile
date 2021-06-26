

LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=kepler-calc
OBJS=kepler-calc.o ../common/mathlib.o ../common/utils.o

include ../Makefile

test:
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO kepler-calc.out
	rm -f FOO

