AUTOMAKE_OPTIONS=foreign no-dependencies
SUBDIRS=de
man_MANS= autovbox.1 rmdtovbox.1 vbox.1 vbox.5 vbox.conf.5 vboxbeep.1 vboxcnvt.1 vboxd.8 vboxd.conf.5 vboxgetty.8 vboxgetty.conf.5 vboxmode.1 vboxrc.5 vboxtcl.5 vboxtoau.1
DISTCLEANFILES=${man_MANS}
EXTRA_DIST= autovbox.man rmdtovbox.man vbox.conf.man vbox.man vboxbeep.man vboxcnvt.man vboxd.conf.man vboxd.man vboxgetty.conf.man vboxgetty.man vboxmode.man vboxrc.man vboxtcl.man vboxtoau.man

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
        	-e "s#\@I4LCONFDIR\@#@I4LCONFDIR@#g" \
        	-e "s#\@LOGFILEDIR\@#@LOGFILEDIR@#g" \
        	-e "s#\@PIDFILEDIR\@#@RUNDIR@#g" \
        	-e "s#\@LCKFILEDIR\@#@LOCKDIR@#g" \
		< $< > $@
