SEED ?= 0xea3495cc76b34acc 

PROG = randwalk
PROGOPTS = -p branch -S $(SEED) -n 7 -s 16 -t 14

SIM = ./simulate
SIMOPTS.0 ?=  --branch-miss-max=21.0
SIMOPTS.1 ?= --branch-miss-max=1.6
SIM_PROGOPTS = -S $(SEED) -n 7 -s 12 -t 10

include common.mk

sim: sim-0 sim-1
test: test-0 test-1

sim-%: $(PROG)
	$(SIM) $(SIMOPTS.$*) randwalk$* -- ./$(PROG) $(SIM_PROGOPTS) -v $*

test-%: $(PROG)
	./$(PROG) $(PROGOPTS) -v $*

correct: $(PROG)
	for i in `seq 10`; do ./$(PROG) -n 7 -s 14 -t 12 || exit 1; done

raport: raport.html tests.png

# vim: ts=8 sw=8
