SUBDIRS=imon imontty-0.3 isdnlog-2.41 isdnbutton-1.1 tcltk
export CC = gcc
export CFLAGS = -O2 -Wall
export ISDN_INCLUDE = ../$(INCLUDES)

all: subdirs

subdirs:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

install: all
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install; done
	@echo
	@echo For using vbox-1.0, you must configure it manually before
	@echo compiling/installing it. See tools/vbox-1.0/INSTALL
	@echo

dummy:


