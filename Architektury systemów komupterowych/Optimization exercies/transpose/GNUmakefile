PROG = transpose
PROGOPTS = -p memory -n 16384

SIM = ./simulate --l1-cache=32768,4,64 --ll-cache=524288,16,64
SIMOPTS.0 ?=
SIMOPTS.1 ?= --l1-miss-max=51.0 --ll-miss-max=8.5
SIM_PROGOPTS = -n 4096

include common.mk

sim: sim-0 sim-1
test: test-0 test-1

sim-%: $(PROG)
	 $(SIM) $(SIMOPTS.$*) $(PROG).c:transpose$* -- ./$(PROG) $(SIM_PROGOPTS) -v $*

test-%: $(PROG)
	./$(PROG) $(PROGOPTS) -v $*

raport: raport.html plot1.png plot2.png plot3.png

# vim: ts=8 sw=8
