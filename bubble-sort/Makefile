
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=bubble-sort
OBJS=bubble-sort.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO bubble-sort.out
	rm -f FOO

