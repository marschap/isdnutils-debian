AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=xisdnload.1
bin_PROGRAMS=xisdnload
man_MANS=xisdnload.1
xisdnload_SOURCES=xisdnload.c xisdnload.bit
xisdnload_LDADD=@X_LIBS@ @X_EXTRA_LIBS@ @X_PRE_LIBS@ -lX11 -lXt -lXaw
noinst_DIST=XISDNLoad.ad

%.1: %.man
	cp $< $@
