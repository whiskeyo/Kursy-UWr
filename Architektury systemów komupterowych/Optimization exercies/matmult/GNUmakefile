PROG = matmult
PROGOPTS = -p memory -n 1024

SIM = ./simulate --l1-cache=32768,4,64 --ll-cache=262144,8,64
SIMOPTS.0 ?= --l1-miss-max=51.0 --ll-miss-max=51.0
SIMOPTS.1 ?= --l1-miss-max=6.5 --ll-miss-max=0.5
SIMOPTS.2 ?= --l1-miss-max=100.0 --ll-miss-max=100.0
SIMOPTS.3 ?= --l1-miss-max=2.25 --ll-miss-max=0.5
SIM_PROGOPTS = -n 256

PLOT_FILES := plot1.gpi plot2.gpi plot1.txt plot2.txt
IMAGES := plot1.png plot2.png

include common.mk

sim: sim-0 sim-1 sim-2 sim-3
test: test-0 test-1 test-2 test-3

sim-%: $(PROG)
	$(SIM) $(SIMOPTS.$*) $(PROG).c:$(PROG)$* -- ./$(PROG) $(SIM_PROGOPTS) -v $*

test-%: $(PROG)
	./$(PROG) $(PROGOPTS) -v $*

plot1.png: plot1.gpi plot1.txt
	gnuplot plot1.gpi

plot2.png: plot1.gpi plot1.txt
	gnuplot plot2.gpi

raport: raport.html plot1.png plot2.png

# vim: ts=8 sw=8
