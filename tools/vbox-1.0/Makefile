
#-------------------------------------------------------------------------#
# vbox                                                                    #
#-------------------------------------------------------------------------#

#-------------------------------------------------------------------------#
# SPOOLDIR                                                                #
#-------------------------------------------------------------------------#
# Verzeichnis, in dem die Daten für `vbox' abgelegt werden. Jeder Benutz- #
# er bekommt in diesem Verzeichnis ein Unterverzeichnis mit seinem Namen  #
# angelegt. In diesem Verzeichniss werden dann noch zusätzlich die Unter- #
# verzeichnisse 'incoming' (für die eingehenden Daten) und 'messages'     #
# (für die Nachrichten die der Anrufer vorgespielt bekommen soll) ange-   #
# legt!                                                                   #
#-------------------------------------------------------------------------#

SPOOLDIR	=	/var/spool/vbox

#-------------------------------------------------------------------------#
# INSTALLSBINDIR                                                          #
#-------------------------------------------------------------------------#
# Verzeichnis in das `vboxgetty' installiert wird (*Nur* für root).       #
#-------------------------------------------------------------------------#

INSTALLSBINDIR	=	/usr/local/sbin

INSTALLXSBINDIR	=	/usr/X11/bin
INSTALLXBINDIR	=	/usr/X11/bin

#-------------------------------------------------------------------------#
# INSTALLSBIN                                                             #
#-------------------------------------------------------------------------#
# Programm mit dem `vboxgetty' installiert wird.                          #
#-------------------------------------------------------------------------#

INSTALLSBIN	=	install -s -o root -g root -m 750

#-------------------------------------------------------------------------#
# INSTALLBINDIR                                                           #
#-------------------------------------------------------------------------#
# Verzeichnis in das `vbox' (message viewer) installiert wird.            #
#-------------------------------------------------------------------------#

INSTALLBINDIR	=	/usr/local/bin

#-------------------------------------------------------------------------#
# INSTALLBIN                                                              #
#-------------------------------------------------------------------------#
# Programm mit dem `vbox' installiert wird.                               #
#-------------------------------------------------------------------------#

INSTALLBIN	=	install -s -o root -g bin -m 755

#-------------------------------------------------------------------------#
# CC & FLAGS                                                              #
#-------------------------------------------------------------------------#

CC		=	gcc
CCFLAGS		=	-i486 -Wall -O2

#-- version --------------------------------------------------------------#

VERSION		=	1.0

#-- all ------------------------------------------------------------------#

all:			ignore
			@echo ""
			@echo "Be sure you are root and then do one ore more of the following:"
			@echo ""
			@echo "make vbox          - Builds the executables..."
			@echo "make install       - Installs the executables..."
			@echo "make install-dir   - Installs the directory \"$(SPOOLDIR)\"..."
			@echo "make install-man   - Installs the man pages..."
			@echo "make clean         - Deletes all backups..."
			@echo "make clear         - Deletes all backups, objects & executables..."
			@echo ""

#-- vbox -----------------------------------------------------------------#

vbox:			ignore
			@if (test -d ./src); \
			then \
			   make -C src executables; \
			else \
			   make executables; \
			fi;

#-- clean ----------------------------------------------------------------#

clean:			ignore
			@if (test -d ./src); \
			then \
			   make -C src make-clean; \
			else \
			   make make-clean; \
			fi;
			@rm -f *~

#-- clean ----------------------------------------------------------------#

clear:			ignore
			@if (test -d ./src); \
			then \
			   make -C src make-clear; \
			else \
			   make make-clear; \
			fi;
			@rm -f *~

#-- install --------------------------------------------------------------#

install:		ignore
			$(INSTALLSBIN) ./bin/vboxgetty $(INSTALLSBINDIR)
			$(INSTALLBIN) ./bin/vbox $(INSTALLBINDIR)
			$(INSTALLBIN) ./bin/rmdcatheader $(INSTALLBINDIR)
			$(INSTALLBIN) ./bin/rmdcutheader $(INSTALLBINDIR)

#-- install-dir ----------------------------------------------------------#

install-dir:		ignore
			mkdir -p -m 755 $(SPOOLDIR)

#-- ignore ---------------------------------------------------------------#

release:		clear
			@cd doc/de; sgml2txt -l -g vbox.sgml
			@cd doc/de; sgml2html -L deutsch -l vbox.sgml
			@cd doc/de; sgml2latex -l -g -a vbox.sgml


ignore:
