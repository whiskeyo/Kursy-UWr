PROGS = bug-1 bug-2 echoclient-thread echoserver-select echoserver-poll
EXTRA-FILES = 

include Makefile.include

LDLIBS += -lpthread

# bug-1: CC += -fsanitize=thread
bug-2: CC += -fsanitize=thread

# vim: ts=8 sw=8 noet
