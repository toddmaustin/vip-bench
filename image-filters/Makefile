
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=
LOCAL_CLEAN=FOO.bmp

PROG=image-filters
OBJS=image-filters.o helpers.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) -s original.bmp FOO.bmp
	$(TARGET_DIFF) FOO.bmp image-filters.out
	rm -f FOO.bmp

