#!/bin/sh -v

	set -e

	install -m 0755 scripts/isdncause                   debian/isdnutils-base/usr/bin/isdncause
	install -m 0644 isdnctrl/README.libipt              debian/isdnutils-base/usr/share/doc/isdnutils-base/
	rm -f             debian/tmp/etc/isdn/eftusers
	# ln -s ../ftpusers debian/isdneurofile/etc/isdn/eftusers
	install -m 0444 Mini-FAQ/isdn-faq.*           debian/isdnutils-doc/usr/share/doc/isdnutils-doc/Mini-FAQ/
	mv debian/tmp/usr/bin/x*                      debian/isdnutils-xtools/usr/bin/
	cp xmonisdn/README                            debian/isdnutils-xtools/usr/share/doc/isdnutils-xtools/README.xmonisdn
	# move some extra stuff from upstream source into debian tree
	cp ipppd/NOTES.IPPPD ipppd/README* debian/README.MPPP debian/ipppd/usr/share/doc/ipppd/
	mkdir -p                                      debian/isdnlog/usr/share/doc/isdnlog
	cp lib/README.Syntax.conffile                 debian/isdnlog/usr/share/doc/isdnlog/Syntax.conffile.de
	cp isdnlog/BUGS	                              debian/isdnlog/usr/share/doc/isdnlog/BUGS.de
	cp isdnlog/FAQ	                              debian/isdnlog/usr/share/doc/isdnlog/FAQ.de
	cp isdnlog/README                             debian/isdnlog/usr/share/doc/isdnlog/README.de
	cp isdnlog/TODO	                              debian/isdnlog/usr/share/doc/isdnlog/TODO.de
	cp isdnlog/contrib/winpopup/winpopup          debian/isdnlog/usr/share/doc/isdnlog/examples/winpopup.de
	cp isdnlog/samples/*                          debian/isdnlog/usr/share/isdn/default/
	rm -f debian/isdnlog/usr/share/isdn/default/*.in

	# not this one
	rm -f debian/isdnlog/usr/share/isdn/default/callerid.conf
	rm -f debian/tmp/etc/isdn/callerid.conf	\
	      debian/tmp/etc/isdn/isdn.conf	\
	      debian/tmp/etc/isdn/rate.conf
	# cp pcbit/README.pt                          debian/isdnactivecards/usr/share/doc/isdnactivecards/README.pcbit.pt
	# extra stuff, not originating from the upstream source
	install -m 0755 debian/isdnconfig           debian/isdnutils-base/usr/sbin/
	install -m 0644 debian/isdnconfig.8         debian/isdnutils-base/usr/share/man/man8/
	install -m 0755 debian/x*-net*	            debian/isdnutils-xtools/etc/isdn/
	install -m 0644 debian/init.d.functions     debian/isdnutils-base/etc/isdn/
	install -m 0444 debian/ioptions		    debian/ipppd/etc/ppp/
	install -m 0755 debian/ip-down.d            debian/ipppd/etc/ppp/ip-down.d/99-ipppd
	install -m 0755 debian/ip-up.d	            debian/ipppd/etc/ppp/ip-up.d/00-ipppd
	rmdir debian/isdnvboxserver/usr/share/doc/isdnvboxserver/examples || true # dont ask
	mv debian/tmp/usr/share/doc/isdnvboxserver/* debian/isdnvboxserver/usr/share/doc/isdnvboxserver/.
	rmdir debian/tmp/usr/share/doc/isdnvboxserver
	# install examples
	cp debian/device.DEVICE debian/isdnutils-base/usr/share/isdn/default/
	for A in debian/ipppd.DEVICE \
		debian/auth-down debian/auth-up ; do \
	    cp $A debian/ipppd/usr/share/isdn/default/; \
	done
	for A in debian/callerid.conf debian/isdnlog.DEVICE; do \
	    cp $A debian/isdnlog/usr/share/isdn/default/; \
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
	install -m 0644 debian/README.HiSax  debian/isdnutils-base/usr/share/doc/isdnutils-base/
	install -m 0644 debian/README.source debian/isdnutils-base/usr/share/doc/isdnutils-base/
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
