#!/bin/sh

# example /etc/ppp/ip-up.d script for use with ipppd and dynamic IP numbers.
# Edit / add to the entries to suit the requirements of the interface.

case "$PPP_IFACE" in
	ippp*)	route add default $PPP_IFACE	# usually necessary
		# The next lines are for masquerading over ippp link.
		# hostname -i should give ip address of local network interface.
		# See comments in /etc/isdn/device.* about firewalling!
		# ipfwadm -I -a accept -W $PPP_IFACE -P tcp -D $PPP_LOCAL smtp ident 1024:65535
		# ipfwadm -I -a accept -W $PPP_IFACE -P udp -D $PPP_LOCAL 1024:65535
		# masquerading rules, for transparently connecting local net to internet
		# ETH_IP=`hostname -i | tr -d ' '`
		# ipfwadm -F -a accept -m -P tcp -S $ETH_IP/24
		# ipfwadm -F -a accept -m -P udp -S $ETH_IP/24
		;;
esac
