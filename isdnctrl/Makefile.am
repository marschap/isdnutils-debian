AUTOMAKE_OPTIONS=foreign no-dependencies
sbin_PROGRAMS=isdnctrl
man_MANS=isdnctrl.8
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in config.h.in stamp-h.in
DISTCLEANFILES=isdnctrl.8
isdnctrl_SOURCES=isdnctrl.c

%.8: %.man
	cp $< $@
