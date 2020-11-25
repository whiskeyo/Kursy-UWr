PROGS = forksort hashdb
EXTRA-FILES = 

include Makefile.include

URL = https://github.com/danielmiessler/SecLists/raw/master/Passwords

download:
	wget $(URL)/xato-net-10-million-passwords.txt
	wget $(URL)/xato-net-10-million-passwords-1000000.txt

.PHONY: download

# vim: ts=8 sw=8 noet
