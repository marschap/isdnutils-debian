## $Id: Makefile,v 2.41 1996/06/23 11:50:19 akool Exp akool $
##
## ISDN accounting for isdn4linux.
##
## Copyright 1995, 1996 by Andreas Kool (akool@Kool.f.EUnet.de)
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
## $Log: Makefile,v $
## Revision 2.41  1996/06/23  11:50:19  akool
##
##

#
# moegliche COPTS - Optionen:
#
#  -DSELECT_FIX  - work-around gegen einen Bug im ISDN subsystem Rev: 1.5
#
#  -DI4l_073	 - work-around gegen einen Bug in allen isdn4linux-Versionen
#  		   _vor_ dem ISDN subsystem Rev: 1.5
#
# ACHTUNG: Ab "i4lk_1.3.97_alpha" werden diese Schalter _nicht_ mehr benoetigt!
#
#  -DSL		 - work-aroung gegen falsch konfigurierte Vermittlungsstellen
#  		   (unterdrueckt alle 1TR6 frames, falls die Post an einen
#		   normalen E-DSS1 Anschluss multilinguale frames schickt)
#
# ACHTUNG: Unter $(LIB) evtl. "-lgdbm" einsetzen
#

# Linux
COPTS       = -O3 # -g # -DSELECT_FIX # -DTEST
CC          = gcc -Wall -pipe $(COPTS)
INCLUDE     = -I./connect -I./tools
LIB         = -ldbm
XDEF        = -ansi -Dlinux -D__i386__ -D_POSIX_SOURCE -D_BSD_SOURCE -D_GNU_SOURCE -DX_LOCALE  -DNO_MESSAGE_CATALOG  -DFUNCPROTO=15 -DNARROWPROTO
XLIB        = /usr/X11/lib/libXm.a /usr/X11/lib/libXpm.a -L/usr/X11/lib -lXt -lXext -lX11 -lc
XINCLUDE    = -I/usr/X11/include -I./client -I./client/Xm

# UnixWare 2.03
#COPTS       = -O
#CC          = /usr/ucb/cc $(COPTS)
#INCLUDE     = -I./connect -I./tools
#LIB         = -lgen -lnsl -lsocket -L/usr/ucblib -ldbm
#XLIB        = /usr/X11/lib/libXmt.a /usr/X11/lib/libXm.a /usr/X11/lib/libXpm.a -L/usr/X11/lib -lXt -lXext -lX11
#XINCLUDE    = -I/usr/X11/include -I./client/Xmt -I.

INSTALL     = install -c -o bin -g bin
VPATH       = .

CONFDIR     = /etc/isdnlog
CONFFILE    = isdnlog.conf
USERFILE    = isdnlog.users

LOGFILE     = /var/log/isdn/calls
PIDFILE     = /var/run/isdnlog.pid

ETCDIR      = /sbin
BINDIR      = /usr/sbin
TMPDIR      = /tmp
X11BIN      = /usr/X11R6/bin
APPDEF      = /usr/X11R6/lib/X11/app-defaults

SERV_PORT   = 20011
SERVICEFILE = /etc/services
AVON        = avon

# sl's Environment
#CC          = gcc -Wall -pipe -g -DSL -DI4l_073 #-DTEST #-pg
#CONFDIR     = /etc/config/isdnlog
#ETCDIR      = /usr/local/sbin
#BINDIR      = /usr/local/bin
#X11BIN      = /usr/local/X11/bin
#

######################################################################
# DON'T EDIT BELOW THIS LINE
######################################################################

VERSION  =      2.41

CFLAGS   =      -DCONFDIR=\"$(CONFDIR)\" -DCONFFILE=\"$(CONFFILE)\"  \
          	-DUSERFILE=\"$(USERFILE)\"  \
                -DPIDFILE=\"$(PIDFILE)\"   -DLOGFILE=\"$(LOGFILE)\"   \
                -DTMPDIR=\"$(TMPDIR)\"     -DVERSION=\"$(VERSION)\"   \
                -DSERV_PORT=$(SERV_PORT)   $(INCLUDE) $(XINCLUDE)

ISDNLOG_OBJS =  isdnlog/isdnlog.o isdnlog/server.o isdnlog/start_prog.o \
                connect/connect.o connect/socket.o tools/tools.o \
                connect/conv_address.o isdnlog/user_access.o \
                isdnlog/test_center.o isdnrep/cheap.o

ISDNREP_OBJS =  isdnrep/isdnrep.o isdnrep/cheap.o tools/tools.o

XISDN_OBJS   =  client/Xm/main_create.o client/Xm/isdn_call.o \
                client/Xm/isdn_list.o client/Xm/isdn_prot.o \
                connect/connect.o client/client.o tools/tools.o \
                connect/socket.o client/Xm/xisdn.o client/isdn_list_update.o \
                client/Xm/xconnect.o connect/conv_address.o \
		client/Xm/isdn_display.o
#		client/Xmt/XmStringCvt.o \
#               client/Xmt/Localize.o client/Xmt/Lookup.o

ISDNLOG      =  bin/isdnlog
ISDNREP      =  bin/isdnrep
XISDN        =  bin/xisdn

MODS         =  *.o */*.o */*/*.o

PROGS        =  $(ISDNLOG) $(ISDNREP)

all:            $(PROGS)

xall:           $(XISDN) all

clean:
		-rm -f $(MODS)

distclean:      clean
		-rm -f $(PROGS)

xdistclean:     clean
		-rm -f $(PROGS) $(XISDN)

install:        all
		@if [ `id -u` != "0" ]; then echo "";echo 'Do "make install" as root!' ;echo ""; false; fi
		-test -d $(ETCDIR) || ( mkdir -p -m 755 $(ETCDIR) )
		$(INSTALL) -m 700 $(ISDNLOG)  $(ETCDIR)
		-test -d $(BINDIR) || ( mkdir -p -m 755 $(BINDIR) )
		$(INSTALL) -m 755 $(ISDNREP)  $(BINDIR)
		-test -d $(CONFDIR) || ( mkdir -p -m 755 $(CONFDIR) )
		-test conf/$(AVON) -ot $(CONFDIR)/$(AVON) || ( $(INSTALL) -m 644 conf/$(AVON) $(CONFDIR) )
		# -rm -f $(CONFDIR)/$(AVON).pag $(CONFDIR)/$(AVON).dir
		@echo ""
		@echo ""
		@echo "Don't forget to create $(CONFDIR)/$(CONFFILE)"
		@echo "and $(CONFDIR)/$(USERFILE)"
		@echo ""
		@echo ""

xinstall:       xall install
		-test -d $(X11BIN) || ( mkdir -p -m 755 $(X11BIN) )
		$(INSTALL) -m 755 $(XISDN)  $(X11BIN)
		-test -d $(APPDEF) || ( mkdir -p -m 755 $(APPDEF) )
		$(INSTALL) -m 644 Isdn  $(APPDEF)
		@(grep isdnlog $(SERVICEFILE) >/dev/null) || \
		(echo "";echo "";echo "Add a line to the file $(SERVICEFILE)" ;echo "";echo ""; \
		echo "isdnlog $(SERV_PORT)/tcp        isdnlog" >> $(SERVICEFILE))
#               @echo ""
#               @echo ""
#               @echo "Don't forget to export XENVIRONMENT=$(APPDEF)/Isdn"
#               @echo ""
#               @echo ""

test:           $(ISDNLOG)
		-rm $(LOGFILE).rep
		$(ISDNLOG) -rm0xfffff /tmp/i 2>/tmp/e
#		$(ISDNLOG) -rm0xfffff /tmp/isdnctrl0 2>/tmp/e

debug:          $(ISDNLOG)
		gdb ../isdnlog/isdnlog -x gdb.deb 2>/tmp/e

distrib:        xdistclean
		cd .. && tar cf /tmp/isdnlog-$(VERSION).tar    \
		  isdnlog-$(VERSION)/Makefile       \
		  isdnlog-$(VERSION)/Isdn           \
		  isdnlog-$(VERSION)/README         \
		  isdnlog-$(VERSION)/NEWS           \
		  isdnlog-$(VERSION)/COPYING        \
		  isdnlog-$(VERSION)/PATCHES        \
		  isdnlog-$(VERSION)/BUGS           \
		  isdnlog-$(VERSION)/conf/$(AVON)   \
		  isdnlog-$(VERSION)/samples        \
		  isdnlog-$(VERSION)/client         \
		  isdnlog-$(VERSION)/tools          \
		  isdnlog-$(VERSION)/isdnlog        \
		  isdnlog-$(VERSION)/isdnrep        \
		  isdnlog-$(VERSION)/connect        \
		  isdnlog-$(VERSION)/xisdnload      \
		  isdnlog-$(VERSION)/contrib	    \
		  isdnlog-$(VERSION)/bin
		gzip -f9 /tmp/isdnlog-$(VERSION).tar
		uuencode /tmp/isdnlog-$(VERSION).tar.gz isdnlog-$(VERSION).tar.gz > /tmp/isdnlog-$(VERSION).uue

xdistrib:
		cd .. && tar cf /tmp/isdnlog-X11-$(VERSION).tar    \
		  isdnlog-$(VERSION)/X11            \
		  isdnlog-$(VERSION)/sounds         \
		  isdnlog-$(VERSION)/doc
		gzip -f9 /tmp/isdnlog-X11-$(VERSION).tar
		uuencode /tmp/isdnlog-X11-$(VERSION).tar.gz isdnlog-X11-$(VERSION).tar.gz > /tmp/isdnlog-X11-$(VERSION).uue

$(ISDNLOG):     $(ISDNLOG_OBJS)
		$(CC) -o $(ISDNLOG) $(LFLAGS) $(ISDNLOG_OBJS) $(LIB)

$(ISDNREP):     $(ISDNREP_OBJS)
		$(CC) -o $(ISDNREP) $(LFLAGS) $(ISDNREP_OBJS) $(LIB)

$(XISDN):       $(XISDN_OBJS)
		$(CC) -o $(XISDN) $(LFLAGS) $(XISDN_OBJS) $(XLIB) $(LIB)


isdnlog/isdnlog.o:         isdnlog/isdnlog.c isdnlog/isdnlog.h tools/tools.h \
			   connect/socket.h

isdnlog/server.o:          isdnlog/server.c  isdnlog/isdnlog.h tools/tools.h \
			   connect/socket.h

isdnlog/start_prog.o:      isdnlog/start_prog.c isdnlog/isdnlog.h tools/tools.h

isdnlog/user_access.o:     isdnlog/user_access.c isdnlog/isdnlog.h tools/tools.h

tools/tools.o:             tools/tools.c     tools/tools.h

isdnrep/isdnrep.o:         isdnrep/isdnrep.c isdnrep/isdnrep.h tools/tools.h

isdnrep/cheap.o:           isdnrep/cheap.c   isdnrep/isdnrep.h tools/tools.h

connect/conv_address.o:    connect/conv_address.c    connect/socket.h tools/tools.h
connect/socket.o:          connect/socket.c          connect/socket.h tools/tools.h
connect/connect.o:         connect/connect.c         connect/socket.h tools/tools.h

client/client.o:           client/client.c           connect/socket.h tools/tools.h
client/isdn_display.o:     client/isdn_display.c     connect/socket.h tools/tools.h
client/isdn_list_update.o: client/isdn_list_update.c tools/tools.h
client/Xm/xconnect.o:     client/Xm/xconnect.c     connect/socket.h tools/tools.h
client/Xm/xisdn.o:        client/Xm/xisdn.c        connect/socket.h tools/tools.h
client/Xm/main_create.o:  client/Xm/main_create.c  tools/tools.h
client/Xm/isdn_call.o:    client/Xm/isdn_call.c    tools/tools.h
client/Xm/isdn_list.o:    client/Xm/isdn_list.c    tools/tools.h
client/Xm/isdn_prot.o:    client/Xm/isdn_prot.c    tools/tools.h
client/Xm/isdn_display.o: client/Xm/isdn_display.c tools/tools.h
# client/Xmt/XmStringCvt.o: client/Xmt/XmStringCvt.c
# client/Xmt/Lookup.o: client/Xmt/Lookup.c
# client/Xmt/Localize.o: client/Xmt/Localize.c
#client/Xmt/All.o: client/Xmt/All.c
#client/Xmt/Create.o: client/Xmt/Create.c
#client/Xmt/Initialize.o: client/Xmt/Initialize.c
#	$(CC) $(XDEF) -c client/Xmt/XmStringCvt.c
#client/Xmt/xconnect.o:     client/Xmt/xconnect.c     connect/socket.h tools/tools.h
#client/Xmt/xisdn.o:        client/Xmt/xisdn.c        connect/socket.h tools/tools.h
#client/Xmt/main_create.o:  client/Xmt/main_create.c  tools/tools.h
#client/Xmt/isdn_call.o:    client/Xmt/isdn_call.c    tools/tools.h
#client/Xmt/isdn_list.o:    client/Xmt/isdn_list.c    tools/tools.h
#client/Xmt/isdn_prot.o:    client/Xmt/isdn_prot.c    tools/tools.h
#client/Xmt/isdn_display.o: client/Xmt/isdn_display.c tools/tools.h
