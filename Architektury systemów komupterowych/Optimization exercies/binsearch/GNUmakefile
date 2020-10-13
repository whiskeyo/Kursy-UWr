SEED ?= 0x5bab3de5da7882fb 

PROG = binsearch
PROGOPTS = -S $(SEED) -n 23 -t 24

SIM = ./simulate
SIMOPTS.0 ?= --l1-miss-max=78.0 --ll-miss-max=50.0 --branch-miss-max=3.0
SIMOPTS.1 ?= --l1-miss-max=36.0 --ll-miss-max=10.5 --branch-miss-max=3.0
SIM_PROGOPTS = -S $(SEED) -n 19 -t 20

include common.mk

sim: sim-0 sim-1
test: test-0 test-1

sim-%: $(PROG)
	 $(SIM) $(SIMOPTS.$*) binsearch$* -- ./$(PROG) $(SIM_PROGOPTS) -v $*

test-%: $(PROG)
	./$(PROG) -p memory $(PROGOPTS) -v $*

correct: $(PROG)
	for i in `seq 10`; do ./$(PROG) -n 20 -t 21 || exit 1; done

raport: raport.html tests.png

# vim: ts=8 sw=8
