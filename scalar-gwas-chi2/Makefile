
LOCAL_CFLAGS=
LOCAL_LIBS=

PROG=gwas-chi2
OBJS=gwas-chi2.o

include ../Makefile

test: $(TARGET_EXE)
	-$(TARGET_SIM) $(TARGET_EXE) > FOO
	-$(TARGET_DIFF) FOO gwas-chi2.out
	rm -f FOO

