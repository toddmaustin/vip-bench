
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=nonlinear-nn
OBJS=nonlinear-nn.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO nonlinear-nn.out
	rm -f FOO

