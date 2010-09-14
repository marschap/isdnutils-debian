AUTOMAKE_OPTIONS=foreign
pkgdocdir=${prefix}/doc/@PACKAGE@
pkgdoc_DATA=vbox.sgml vbox.txt
DISTCLEANFILES=${doc_DATA}
MAINTAINERCLEANFILES=Makefile.in

%: %.in
	sed \
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
