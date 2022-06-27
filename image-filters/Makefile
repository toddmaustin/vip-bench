
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=
LOCAL_CLEAN=FOO.bmp

PROG=image-filters
OBJS=image-filters.o helpers.o ../common/utils.o ../common/mathlib.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin ./$(TARGET_EXE) -s original.bmp FOO.bmp
	$(TARGET_DIFF) FOO.bmp image-filters.out
	rm -f FOO.bmp
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin ./$(TARGET_EXE) -e original.bmp FOO.bmp
	$(TARGET_DIFF) FOO.bmp image-filters1.out
	rm -f FOO.bmp
