
LOCAL_CFLAGS= -I../common
LOCAL_LIBS=

PROG=pagerank
OBJS=graph.o main.o pagerank.o pageutil.o ../common/utils.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(eval SHELL:=/bin/bash) OZONE_CONTRACT=../keyfiles/dataowner-keyenc.bin OZONE_DEBUGKEY=../keyfiles/dataowner-keybin.bin ./$(TARGET_EXE) --fmt DIMACS --style pull --df 0.85 --filename r1000a.dimacs > FOO 
	$(TARGET_DIFF) FOO pagerank.out
	rm -f FOO

