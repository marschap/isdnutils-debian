AUTOMAKE_OPTIONS=foreign no-dependencies
man_MANS=callerid.conf.5 isdn.conf.5 isdnformat.5 isdnlog.5 isdnlog.8 isdnlog.users.5 isdnrep.8 isdntime.5
DISTCLEANFILES=${man_MANS}
EXTRA_DIST=callerid.conf.man isdn.conf.man isdnformat.man isdnlog.man isdnlog.users.man isdnrep.man isdntime.man

%.1 %.5 %.8: %.man
	MANDATE=`grep CHECKIN $< | awk '{print $4}'`    
	sed \
		-e "s#\@MANDATE\@#${MANDATE}#g" \
		-e "s#\@I4LVERSION\@#@VERSION@#g" \
        	-e "s#\@BINDIR\@#@bindir@#g" \
        	-e "s#\@SBINDIR\@#@sbindir@#g" \
        	-e "s#\@VERSION\@#@VERSION@#g" \
        	-e "s#\@VERDATE\@#@VERDATE@#g" \
        	-e "s#\@PACKAGE\@#@PACKAGE@#g" \
        	-e "s#\@SPOOLDIR\@#@SPOOLDIR@#g" \
        	-e "s#\@I4LCONFDIR\@#@SYSCONFDIR@#g" \
        	-e "s#\@LOGFILEDIR\@#@LOGFILEDIR@#g" \
        	-e "s#\@PIDFILEDIR\@#@PIDFILEDIR@#g" \
        	-e "s#\@LCKFILEDIR\@#@LCKFILEDIR@#g" \
		< $< > $@
