AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in 
noinst_DIST=README.pt 
pcbitctrl_SOURCES=pcbitctl.c convhexbin.c
sbin_PROGRAMS=pcbitctrl
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -I@KERNEL@/drivers/isdn/pcbit -c $< -o $@   

