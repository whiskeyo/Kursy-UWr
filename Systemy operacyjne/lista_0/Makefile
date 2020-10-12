CC = gcc -g
CFLAGS = -Og -Wall
CPPFLAGS = -Iinclude -DLINUX -D_GNU_SOURCE 
LDFLAGS = -Llibapue -lapue

LIBAPUE = libapue/libapue.a 
LIBAPUE_SRC = $(wildcard libapue/*.c)
LIBAPUE_OBJ = $(patsubst %.c,%.o,$(LIBAPUE_SRC))

PROGS =	1_ls 2_cat

all: $(PROGS)

%: %.c $(LIBAPUE)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

$(LIBAPUE): $(LIBAPUE_OBJ)
	$(AR) rc $@ $^

clean:
	rm -f $(PROGS) *.o *~
	rm -f $(LIBAPUE) $(LIBAPUE_OBJ)

# vim: ts=8 sw=8 noet
