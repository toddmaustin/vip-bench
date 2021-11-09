
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=
LOCAL_CLEAN=homesales.db

PROG=filtered-query
OBJS=filtered-query.o kissdb.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) > FOO 
	$(TARGET_DIFF) FOO filtered-query.out
	rm -f FOO

