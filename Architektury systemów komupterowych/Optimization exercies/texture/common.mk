UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CC = gcc -g
endif
ifeq ($(UNAME), Darwin)
CC = clang -g
endif
ifeq ($(UNAME), FreeBSD)
CC = gcc -g
endif

ifneq ($(shell pkg-config --modversion --silence-errors papi),)
PAPI_CFLAGS := $(shell pkg-config --cflags papi) -DPAPI=1
PAPI_LDLIBS := $(shell pkg-config --libs papi)
else
PAPI_CFLAGS := $(shell pkg-config --cflags papi) -DPAPI=0
$(info Please install libpapi-dev package to enable PMU counters!)
endif

OFLAGS ?= -O2
WFLAGS ?= -Wall -Wextra -Wno-unused -Wno-unused-result
CFLAGS = -std=gnu11 $(OFLAGS) $(WFLAGS) $(PAPI_CFLAGS)
LDLIBS = $(PAPI_LDLIBS)

all: $(PROG)

$(PROG): $(PROG).o main.o common.o
$(PROG).o: $(PROG).c common.h $(PROG).h $(PROG)_impl.h
main.o: main.c common.h $(PROG).h
common.o: common.c common.h

%.html: %.md
	markdown $< > $@

%.png: %.gp %.dat
	gnuplot $< > $@

clean:
	@rm -vf $(PROG) *.o *.html *.png *.out *~

.PHONY: all raport clean
# vim: ts=8 sw=8
