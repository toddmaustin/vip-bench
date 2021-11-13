
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=knapsack
OBJS=knapsack.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO knapsack.out
	rm -f FOO

