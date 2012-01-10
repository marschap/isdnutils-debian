#!/bin/sh -v

	set -e

	rm -f             debian/tmp/etc/isdn/eftusers

	# not this one
	rm -f debian/isdnlog/usr/share/isdn/default/callerid.conf
	rm -f debian/tmp/etc/isdn/callerid.conf	\
	      debian/tmp/etc/isdn/isdn.conf	\
	      debian/tmp/etc/isdn/rate.conf

	rmdir debian/isdnvboxserver/usr/share/doc/isdnvboxserver/examples || true # dont ask
	mv debian/tmp/usr/share/doc/isdnvboxserver/* debian/isdnvboxserver/usr/share/doc/isdnvboxserver/.
	rmdir debian/tmp/usr/share/doc/isdnvboxserver
	# install examples
	cp debian/device.DEVICE debian/isdnutils-base/usr/share/isdn/default/
	for A in debian/ipppd.DEVICE \
		debian/auth-down debian/auth-up ; do \
	    cp $A debian/ipppd/usr/share/isdn/default/; \
	done
	mkdir debian/isdnvboxserver/usr/share/doc/isdnvboxserver/examples || true # dont ask
        # The $A's below need to become $$A if this code is placed back into the makefile.
	for A in standard.tcl beep.msg standard.msg timeout.msg vbox.conf vboxd.conf; do \
	    rm -f debian/isdnvboxserver/usr/share/isdn/default/$A debian/isdnvboxserver/usr/share/isdnvboxserver/default/$A; \
	    cp vbox/examples/$A.example debian/isdnvboxserver/usr/share/isdnvboxserver/default/$A; \
	    ln -s ../../isdnvboxserver/default/$A debian/isdnvboxserver/usr/share/isdn/default/$A; \
	done
        install -m 0755 debian/vboxmail.enhanced debian/isdnvboxserver/usr/share/doc/isdnvboxserver/
	install -m 0755 debian/grepconfig.pl debian/isdnutils-base/usr/share/doc/isdnutils-base/examples/
	install -m 0644 debian/HOWTO	     debian/isdnutils-base/usr/share/doc/isdnutils-base/
	ln -s HOWTO                          debian/isdnutils-base/usr/share/doc/isdnutils-base/CONFIG
	ln -s HOWTO                          debian/isdnutils-base/usr/share/doc/isdnutils-base/HOWTO.isdnutils
	install -m 0444 debian/ioptions      debian/ipppd/etc/ppp/
	install -m 0644 isdnlog/rate-*.dat                 debian/isdnlog-data/usr/share/isdn/
	install -m 0644 isdnlog/zone-*.cdb                 debian/isdnlog-data/usr/share/isdn/
	install -m 0644 debian/isdnlog.logrotate.d	   debian/isdnlog/etc/logrotate.d/isdnlog
	# not admin tools IMHO
	mv debian/tmp/usr/sbin/imon*               debian/tmp/usr/bin/.
	mv debian/tmp/usr/share/man/man8/imon.8    debian/tmp/usr/share/man/man1/imon.1
	mv debian/tmp/usr/share/man/man8/imontty.8 debian/tmp/usr/share/man/man1/imontty.1
	# this _is_
	mv debian/tmp/usr/bin/vboxmail             debian/tmp/usr/sbin/.
