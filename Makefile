include ../config.mk

#
# END of user-modifiable variables.
#

ifeq ($(MODE), na)
TARGET_CFLAGS = $(NA_CFLAGS)
TARGET_LIBS = $(NA_LIBS)
TARGET_SIM = $(NA_SIM)
TARGET_DIFF = $(NA_DIFF)
TARGET_EXE = $(PROG).na
else ifeq ($(MODE), do)
TARGET_CFLAGS = $(DO_CFLAGS)
TARGET_LIBS = $(DO_LIBS)
TARGET_SIM = $(DO_SIM)
TARGET_DIFF = $(DO_DIFF)
TARGET_EXE = $(PROG).do
else ifeq ($(MODE), enc)
TARGET_CFLAGS = $(ENC_CFLAGS)
TARGET_LIBS = $(ENC_LIBS)
TARGET_SIM = $(ENC_SIM)
TARGET_DIFF = $(ENC_DIFF)
TARGET_EXE = $(PROG).enc
else
# default is native
TARGET_CFLAGS = $(NA_CFLAGS)
TARGET_LIBS = $(NA_LIBS)
TARGET_SIM = $(NA_SIM)
TARGET_DIFF = $(NA_DIFF)
TARGET_EXE = $(PROG).na
endif

CFLAGS = -std=c++11 -Wall $(OPT_CFLAGS) $(TARGET_CFLAGS) $(LOCAL_CFLAGS)
LIBS = $(LOCAL_LIBS) $(TARGET_LIBS)

build: $(TARGET_EXE)

%.o: %.cpp
ifeq ($(MODE), na)
	$(CXX) $(CFLAGS) -DVIP_NA_MODE -o $@ -c $<
else ifeq ($(MODE), do)
	$(CXX) $(CFLAGS) -DVIP_DO_MODE -o $@ -c $<
else ifeq ($(MODE), enc)
	$(CXX) $(CFLAGS) -DVIP_ENC_MODE -o $@ -c $<
else
	$(error MODE is not defined (add: MODE={na|do|enc}).)
endif

%.o: %.c
ifeq ($(MODE), na)
	$(CC) $(CFLAGS) -DVIP_NA_MODE -o $@ -c $<
else ifeq ($(MODE), do)
	$(CC) $(CFLAGS) -DVIP_DO_MODE -o $@ -c $<
else ifeq ($(MODE), enc)
	$(CC) $(CFLAGS) -DVIP_ENC_MODE -o $@ -c $<
else
	$(error MODE is not defined (add: MODE={na|do|enc}).)
endif

$(TARGET_EXE): $(OBJS)
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(PROG).na $(PROG).do $(PROG).enc *.o core FOO

