AUTOMAKE_OPTIONS=foreign no-dependencies
sbin_PROGRAMS=icnctrl
man_MANS=icnctrl.8
icnctrl.o: icnctrl.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -I@KERNEL@/drivers/isdn/icn -c $< -o $@  
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
DISTCLEANFILES=icnctrl.8

%.8: %.man.in
	MANDATE=`grep CHECKIN $< | awk '{print $4}'`    
	sed \
		-e "s#\@I4LVERSION\@#@VERSION@#g" \
		-e "s#\@MANDATE\@#${MANDATE}#g" \
		< $< > $@
