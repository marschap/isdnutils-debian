AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=loopctrl.8
man_MANS=loopctrl.8
sbin_PROGRAMS=loopctrl
icnctrl.o: icnctrl.c
        $(CC) $(CFLAGS) -nostdinc  -I@KERNEL@/drivers/isdn -c $< -o $@   

%.8: %.man.in
	MANDATE=`grep CHECKIN $< | awk '{print $4}'`    
	sed \
		-e "s#\@I4LVERSION\@#@VERSION@#g" \
		-e "s#\@MANDATE\@#${MANDATE}#g" \
		< $< > $@
