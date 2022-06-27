
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=lda
OBJS=lda.o ../common/utils.o ../common/mathlib.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin $(TARGET_EXE) > FOO
	$(TARGET_DIFF) FOO lda.out
	rm -f FOO

