# $Id: Makefile,v 1.4 1996/06/09 13:11:41 root Exp $

include ../Makefile

#-- Do not edit ----------------------------------------------------------#

CFLAGS		=	$(CCFLAGS) \
			-DSPOOLDIR="\"$(SPOOLDIR)\"" \
			-DVERSION="\"$(VERSION)\""

executables:		make-vboxgetty make-vbox \
			make-catheader make-cutheader

#-- rmdcatheader ---------------------------------------------------------#

CATHEADEROBJS	=	rmdcatheader.o

make-catheader:		$(CATHEADEROBJS)
			$(CC) $(CATHEADEROBJS) -o ../bin/rmdcatheader

#-- rmdcutheader ---------------------------------------------------------#

CUTHEADEROBJS	=	rmdcutheader.o

make-cutheader:		$(CUTHEADEROBJS)
			$(CC) $(CUTHEADEROBJS) -o ../bin/rmdcutheader

#-- vboxgetty ------------------------------------------------------------#

VBOXGETTYOBJS	=	vbox.o modem.o log.o lock.o voice.o vboxrc.o

make-vboxgetty:		$(VBOXGETTYOBJS)
			$(CC) $(VBOXGETTYOBJS) -o ../bin/vboxgetty

#-- vbox -----------------------------------------------------------------#

VBOXLIBS	=	-lncurses

VBOXOBJS	=	vboxview.o vboxrc.o

make-vbox:		$(VBOXOBJS)
			$(CC) $(VBOXOBJS) $(VBOXLIBS) -o ../bin/vbox

#-- install --------------------------------------------------------------#


#-- cleaning -------------------------------------------------------------#

make-clean:	ignore
		@rm -f *~

make-clear:	make-clean
		@rm -f *.[oas] ../bin/v* ../bin/rmd*

#-- end -------------------------------------------------------------------#
