SUBDIRS=ipppd.2.2.0f ipppstats
export CC = gcc
export CFLAGS = -O2 -Wall
export BINDIR = $(SBINDEST)
export MANDIR = $(MANDEST)

all: subdirs

subdirs:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

install: all
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install; done

dummy:


