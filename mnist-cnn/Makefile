
LOCAL_CFLAGS= -Wno-unused-function -Wno-unused-result -Wno-strict-aliasing -I../common
LOCAL_LIBS=

PROG=mnist-cnn
OBJS=mnist-cnn.o kautodiff.o kann.o kann_data.o ../common/mathlib.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) -i mnist-cnn.kan mnist-test-x.knd > FOO
	-$(TARGET_DIFF) FOO mnist-cnn.out
	rm -f FOO

