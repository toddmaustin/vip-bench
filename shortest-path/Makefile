
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=shortest-path
OBJS=shortest-path.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO shortest-path.out
	rm -f FOO

