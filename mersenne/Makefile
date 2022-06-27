
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=mersenne
OBJS=mersenne.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO mersenne.out
	rm -f FOO

