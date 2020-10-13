PROG = texture
PROGOPTS = -p memory -S 0xdeadc0de -t 65536

SIM = ./simulate --ll-cache 1048576,16,64
SIMOPTS.0 ?= 
SIMOPTS.1 ?= --ll-miss-max=15.0
SIM_PROGOPTS = -S 0xdeadc0de -t 8192

include common.mk

sim: sim-0 sim-1
test: test-0 test-1

sim-%: $(PROG)
	$(SIM) $(SIMOPTS.$*) query_$* -- ./$(PROG) $(SIM_PROGOPTS) -v $*

test-%: $(PROG)
	./$(PROG) $(PROGOPTS) -v $*

correct: $(PROG)
	for i in `seq 10`; do ./$(PROG) -t 8192 || exit 1; done

raport: tests.png

# vim: ts=8 sw=8
