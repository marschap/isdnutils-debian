#!/bin/sh -x

	set -e

	mv debian/isdnutils-xtools.files.orig debian/isdnutils-xtools.files
	if [ -s debian/isdneurofile/etc/init.d/eftd.sh ]; then mv debian/isdneurofile/etc/init.d/eftd.sh debian/isdneurofile/etc/init.d/isdneurofile; fi
	mv debian/isdnutils-doc/usr/share/doc/isdnutils/* debian/isdnutils-doc/usr/share/doc/isdnutils-doc/
	rmdir debian/isdnutils-doc/usr/share/doc/isdnutils
	cp pppdcapiplugin/README           debian/pppdcapiplugin/usr/share/doc/pppdcapiplugin/
	cp pppdcapiplugin/examples/*       debian/pppdcapiplugin/usr/share/doc/pppdcapiplugin/examples/
        # is installed into /etc/drdsl/ already:
        rm -f debian/pppdcapiplugin/usr/share/doc/pppdcapiplugin/examples/adsl.conf
	chmod -x debian/pppdcapiplugin/etc/ppp/peers/isdn/*
	install -m 0644 capiinit/capi.conf debian/isdnactivecards/etc/isdn/
	# ln -s ../isdnutils-doc/FAQ       debian/isdnutils-doc/usr/share/doc/isdnutils/
	# ln -s ../isdnutils-doc/Mini-FAQ  debian/isdnutils-doc/usr/share/doc/isdnutils/
	mkdir -p debian/isdneurofile/usr/share/doc/isdneurofile/scripts
	install -m 0644 eurofile/doc/README debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/doc/FAQ    debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/doc/INTERWORKING* debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/doc/PROBLEME.eftd debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/doc/design.txt debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/doc/*.1 debian/isdneurofile/usr/share/man/man1/
	install -m 0644 eurofile/doc/*.5 debian/isdneurofile/usr/share/man/man5/
	install -m 0644 eurofile/doc/*.8 debian/isdneurofile/usr/share/man/man8/
	install -m 0644 eurofile/CHANGES debian/isdneurofile/usr/share/doc/isdneurofile/changelog
	install -m 0644 eurofile/INSTALL debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/README* debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0644 eurofile/TODO debian/isdneurofile/usr/share/doc/isdneurofile/
	install -m 0755 eurofile/scripts/check_system debian/isdneurofile/usr/share/doc/isdneurofile/scripts/
	install -m 0755 eurofile/scripts/eft_useradd debian/isdneurofile/usr/share/doc/isdneurofile/scripts/
	install -m 0755 eurofile/scripts/ix25test debian/isdneurofile/usr/share/doc/isdneurofile/scripts/
	install -m 0755 eurofile/scripts/eftd.sh debian/isdneurofile/usr/share/doc/isdneurofile/scripts/

	install -m 0644 debian/isdnutils-README.transitional debian/isdnutils/usr/share/doc/isdnutils/README.transitional
	# ln -s isdnutils-base debian/isdnutils-base/usr/share/doc/isdnutils

	# already in ./Mini-FAQ/
	rm -f usr/share/doc/isdnutils-doc/isdn-faq.txt.gz
	# clean up the mess left behind by moving out stuff to other packages
	rm -f debian/tmp/etc/isdn/isdnlog.isdnctrl0.options
	rm -f debian/tmp/usr/share/man/man1/xmonisdn.1x*
