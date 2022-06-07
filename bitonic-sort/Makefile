
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=bitonic-sort
OBJS=bitonic-sort.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO bitonic-sort.out
	rm -f FOO

