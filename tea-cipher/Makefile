
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=tea-cipher
OBJS=tea-cipher.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO tea-cipher.out
	rm -f FOO

