VERSION=2.0
SUBDIRS=isdn icn teles pcbit ppp4i4k doc tools
# Where to install the mapages
export MANDEST = /usr/man
# Where to install control programs
export SBINDEST = /sbin
# Where to install the modules ($KERNELDIR/modules)
export KERNELDIR = /usr/src/linux
export CC = gcc
export PCFLAGS = -O2 -Wall

all: subdirs

devices:
	@bash chkroot.sh create devices
	@bash makedev.sh

subdirs:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f *.[iso] *~ core tools/*~

dist: clean
	@cd .. && tar cvzf distisdn/isdn4k-utils-${VERSION}.tar.gz isdn4k-utils-${VERSION}
	@echo isdn4k-utils-${VERSION}.tar.gz > ../distisdn/uversion
	@rm -f ../distisdn/uAnnounce

install: all
	@bash chkroot.sh install the package
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install; done

install-man: ;
	set -e; $(MAKE) -C doc install

