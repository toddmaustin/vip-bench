
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=mersenne
OBJS=mersenne.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO mersenne.out
	rm -f FOO

