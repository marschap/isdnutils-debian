AUTOMAKE_OPTIONS=foreign no-dependencies
man_MANS= ttyI.4 isdninfo.4 isdn_audio.4 isdn_cause.7
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 configure Makefile.in 
DISTCLEANFILES=${man_MANS}
EXTRA_DIST= ttyI.man.in isdninfo.man.in isdn_audio.man.in isdn_cause.man.in Configure.help

%.4 %.7: %.man.in
	MANDATE=`grep CHECKIN $< | awk '{print $4}'`    
	sed \
		-e "s#\@MANDATE\@#${MANDATE}#g" \
		-e "s#\@I4LVERSION\@#@VERSION@#g" \
        	-e "s#\@I4LCONFDIR\@#@I4LCONFDIR@#g" \
		< $< > $@
