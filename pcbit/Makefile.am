AUTOMAKE_OPTIONS=foreign no-dependencies
sbin_PROGRAMS=pcbitctrl
man_MANS=pcbitctrl.8
noinst_DIST=README.pt 
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
pcbitctrl_SOURCES=pcbitctl.c convhexbin.c
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -I@KERNEL@/drivers/isdn/pcbit -c $< -o $@   

