AUTOMAKE_OPTIONS=foreign no-dependencies
sbin_PROGRAMS=avmcapictrl
man_MANS=avmcapictrl.8
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=avmcapictrl.8

%.8: %.man.in
	MANDATE=`grep CHECKIN $< | awk '{print $4}'`    
	sed \
		-e "s#\@I4LVERSION\@#@VERSION@#g" \
		-e "s#\@MANDATE\@#${MANDATE}#g" \
		< $< > $@
