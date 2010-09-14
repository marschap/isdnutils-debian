AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=install-sh missing mkinstalldirs aclocal.m4 \
	configure Makefile.in config.h.in stamp-h.in
sbin_PROGRAMS=ipppd
man_MANS=ipppd.8
EXTRA_DIST=md4.h chap_ms.h md4.c chap_ms.c isexpired.c
ipppd_SOURCES=main.c magic.c fsm.c lcp.c ipcp.c upap.c chap.c md5.c \
	ccp.c ipxcp.c auth.c options.c sys-linux.c cbcp.c callout.h \
	pathnames.h patchlevel.h chap.h md5.h ipxcp.h cbcp.h isexpired.c
ipppd_LDADDS=@MSCHAPLIB@ shadow.a
