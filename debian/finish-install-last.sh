#!/bin/sh -x

	set -e

	if [ -s debian/isdneurofile/etc/init.d/eftd.sh ]; then mv debian/isdneurofile/etc/init.d/eftd.sh debian/isdneurofile/etc/init.d/isdneurofile; fi
        # is installed into /etc/drdsl/ already:
        rm -f debian/pppdcapiplugin/usr/share/doc/pppdcapiplugin/examples/adsl.conf
	install -m 0644 capiinit/capi.conf debian/capiutils/etc/isdn/
	install -m 0644 debian/capi.conf debian/capiutils/etc/isdn/

	# already in ./Mini-FAQ/
	rm -f usr/share/doc/isdnutils-doc/isdn-faq.txt.gz
	# clean up the mess left behind by moving out stuff to other packages
	rm -f debian/tmp/etc/isdn/isdnlog.isdnctrl0.options
	rm -f debian/tmp/usr/share/man/man1/xmonisdn.1x*
