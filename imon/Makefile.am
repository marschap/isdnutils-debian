AUTOMAKE_OPTIONS=foreign no-dependencies
bin_PROGRAMS=imon
man_MANS=imon.8
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in stamp-h.in config.h.in stamp-h.in
DISTCLEANFILES=imon.8
%.8: %.man
	cp $< $@
