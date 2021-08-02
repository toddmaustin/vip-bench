
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=test-linear-reg
OBJS=test-linear-reg.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO test-linear-reg.out
	rm -f FOO

