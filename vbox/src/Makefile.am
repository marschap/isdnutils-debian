AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=Makefile.in config.h config.h.in stamp-h.in
INCLUDES=-DGETTYRC=\"@I4LCONFDIR@/vboxgetty.conf\" \
	-DVBOXDRC=\"@I4LCONFDIR@/vboxd.conf\" \
	-DPLAY=\"${bindir}/vboxplay\" -DBINDIR=\"@bindir@\"

noinst_LIBRARIES=libvbox.a
libvbox_a_SOURCES=libvbox.c libvbox.h

sbin_PROGRAMS=vboxgetty vboxd
bin_PROGRAMS=vbox vboxconvert vboxctrl vboxbeep

vboxgetty_SOURCES=vboxgetty.c vboxgetty.h init.c init.h log.c log.h \
		perms.c perms.h lock.c lock.h streamio.c modem.c modem.h \
		script.c script.h voice.c voice.h lists.c lists.h \
		rcvbox.c rcvbox.h rcgetty.c rcgetty.h
vboxgetty_LDADD=-lvbox -L. @TCLLIB@

vbox_SOURCES=vbox.c streamio.c streamio.h
vbox_LDADD=-lvbox -L. @NCLIB@

vboxconvert_SOURCES=vboxconvert.c
vboxconvert_LDADD=-lvbox -L.

vboxctrl_SOURCES=vboxctrl.c vboxctrl.h
vboxctrl_LDADD=-lvbox -L.

vboxbeep_SOURCES=vboxbeep.c
vboxbeep_LDADD=-lvbox -L.

vboxd_SOURCES=vboxd.c vboxd.h streamio.c streamio.h
vboxd_LDADD=-lvbox  -L.

install-exec-local:
	ln ${bindir}/vboxconvert ${bindir}/autovbox
	ln ${bindir}/vboxconvert ${bindir}/vboxtoau
	ln ${bindir}/vboxconvert ${bindir}/rmdtovbox
	ln ${bindir}/vboxconvert ${bindir}/vboxmode
