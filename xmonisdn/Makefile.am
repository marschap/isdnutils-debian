AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=xmonisdn.1
bin_PROGRAMS=xmonisdn
man_MANS=xmonisdn.1
xmonisdn_SOURCES=Net.c Net.h NetP.h xmonisdn.c netactive netactiveout netinactive netstart netstop netwaiting
xmonisdn_LDADD=@X_LIBS@ @X_EXTRA_LIBS@ @X_PRE_LIBS@ -lX11 -lXt -lXaw
noinst_DIST=README

xmonisdn.1: xmonisdn.man
	cp $< $@
