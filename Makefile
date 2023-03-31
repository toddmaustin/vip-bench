define HELP_TEXT
Please choose one of the following target
  config-vip     - configure VIP-Bench to target the VIP-Bench functional library
  config-meso    - configure VIP-Bench to target Agita Labs' Mesosphere secure computation SDK
  config-seal    - configure VIP-Bench to target Microsoft's SEAL Homomorphic Encryption library
  run-tests      - clean, build, and test all benchmarks in all target modes (NA,DO,ENC)
  all-clean      - clean all benchmark directories
endef

export HELP_TEXT

error:
	@echo "$$HELP_TEXT"

#
# END of user-modifiable variables
#
BMARKS = bitonic-sort boyer-moore-search bubble-sort distinctness distinctness-Onlog2n edit-distance eulers-number-approx fft-int filtered-query flood-fill flood-fill-On2 gcd-list gradient-descent grad-x image-filters kadane kepler-calc knapsack lda linear-reg mersenne minspan mnist-cnn nonlinear-nn nr-solver pagerank parrando primal-test rabinkarp-search rad-to-degree randshell-sort recommender set-intersect shortest-path string-search tea-cipher triangle-count

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

CFLAGS = -std=c++11 -Wall $(OPT_CFLAGS) -Wno-strict-aliasing $(TARGET_CFLAGS) $(LOCAL_CFLAGS)
LIBS = $(LOCAL_LIBS) $(TARGET_LIBS)

build: $(TARGET_EXE)

%.o: %.cpp
ifeq ($(MODE), na)
	$(CXX) $(CFLAGS) -DVIP_NA_MODE -o $(notdir $@) -c $<
else ifeq ($(MODE), do)
	$(CXX) $(CFLAGS) -DVIP_DO_MODE -o $(notdir $@) -c $<
else ifeq ($(MODE), enc)
	$(CXX) $(CFLAGS) -DVIP_ENC_MODE -o $(notdir $@) -c $<
else
	$(error MODE is not defined (add: MODE={na|do|enc}).)
endif

%.o: %.c
ifeq ($(MODE), na)
	$(CC) $(CFLAGS) -DVIP_NA_MODE -o $(notdir $@) -c $<
else ifeq ($(MODE), do)
	$(CC) $(CFLAGS) -DVIP_DO_MODE -o $(notdir $@) -c $<
else ifeq ($(MODE), enc)
	$(CC) $(CFLAGS) -DVIP_ENC_MODE -o $(notdir $@) -c $<
else
	$(error MODE is not defined (add: MODE={na|do|enc}).)
endif

$(TARGET_EXE): $(OBJS)
	$(LD) $(CFLAGS) -o $@ $(notdir $^) $(LIBS)

clean:
	rm -f $(PROG).na $(PROG).do $(PROG).enc *.o core mem.out *.log FOO $(LOCAL_CLEAN)


#
# top-level Makefile interfaces
#

config-vip:
	@echo "Configuring VIP-Bench for VIP functional library..."
	ln -sf configs/config.mk.vip config.mk
	ln -sf configs/config.h.vip config.h

config-mesa:
	@echo "Configuring VIP-Bench for Agita Labs' Mesosphere SDK..."
	ln -sf configs/config.mk.meso config.mk
	ln -sf configs/config.h.meso config.h

config-seal:
	@echo "Configuring VIP-Bench for Microsoft SEAL HE library..."
	ln -sf configs/config.mk.seal config.mk
	ln -sf configs/config.h.seal config.h

run-tests:
	@for _BMARK in $(BMARKS) ; do \
	  for _MODE in na do enc ; do \
	    cd $$_BMARK ; \
	    echo "--------------------------------" ; \
	    echo "Running "$$_BMARK" in MODE="$$_MODE ; \
	    echo "--------------------------------" ; \
	    make MODE=$$_MODE clean build test ; \
	    cd .. ; \
	  done \
	done

all-clean:
	@for _BMARK in $(BMARKS) ; do \
	  for _MODE in na do enc ; do \
	    cd $$_BMARK ; \
	    echo "--------------------------------" ; \
	    echo "Running "$$_BMARK" in MODE="$$_MODE ; \
	    echo "--------------------------------" ; \
	    make MODE=$$_MODE clean ; \
	    cd .. ; \
	  done \
	done

