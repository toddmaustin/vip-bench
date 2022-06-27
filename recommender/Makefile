
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=
LOCAL_CLEAN=results.txt

PROG=recommender
OBJS=main.o parser.o predictor.o rating.o ../common/utils.o ../common/mathlib.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin ./$(TARGET_EXE) ratings-small.csv 2 0.001 0.01 > FOO 
	$(TARGET_DIFF) FOO recommender.out
	rm -f FOO

