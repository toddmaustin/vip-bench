
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=kissdb
OBJS=kissdb.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO kissdb.out
	rm -f FOO

