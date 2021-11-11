
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=recommender
OBJS=main.o parser.o predictor.o rating.o ../common/utils.o ../common/mathlib.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) ./$(TARGET_EXE) ratings.csv 2 0.001 0.01 > FOO 
	$(TARGET_DIFF) FOO recommender.out
	rm -f FOO

