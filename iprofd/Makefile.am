AUTOMAKE_OPTIONS=foreign no-dependencies
sbin_PROGRAMS=iprofd
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=iprofd.8

%.8: %.man
	cp $< $@
