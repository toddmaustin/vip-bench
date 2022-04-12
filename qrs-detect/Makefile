
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=qrs-detect
OBJS=qrs-detect.o panTompkins.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) test_input.txt FOO 
	$(TARGET_DIFF) FOO qrs-detect.out
	rm -f FOO

