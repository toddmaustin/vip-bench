
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=rabinkarp-search
OBJS=rabinkarp-search.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO rabinkarp-search.out
	rm -f FOO

