#!/bin/sh

# example /etc/ppp/ip-up.d script for use with ipppd and dynamic IP numbers.
# Edit / add to the entries to suit the requirements of the interface.

# Use the network to enable the IP dynamic hack to reset old connections
# (otherwise the RST-provoking packet can't get out).
# For a smaller set of dynamic numbers:
# PPP_NET=`echo $PPP_LOCAL | sed 's,\.[0-9]*$,.0/24,'`
# in case your ISP has a large pool of dynamic numbers (should be OK):
PPP_NET=`echo $PPP_LOCAL | sed 's,\.[0-9]*\.[0-9]*$,.0.0/16,'`

case "$PPP_IFACE" in
	ippp0)	route del default
		# route add default netmask 0 $PPP_IFACE   # usually necessary
                route add default netmask 0 gw $PPP_REMOTE # usually necessary
		# The next lines are for simple firewalling.
		# See comments in /etc/isdn/device.* about firewalling!
		#ipchains -D input -j DENY   -i $PPP_IFACE 2>/dev/null
		#ipchains -A input -j ACCEPT -i $PPP_IFACE -p TCP  -d $PPP_NET 1000:
		#ipchains -A input -j ACCEPT -i $PPP_IFACE -p TCP  -d $PPP_NET ssh
		#ipchains -A input -j ACCEPT -i $PPP_IFACE -p TCP  -d $PPP_NET smtp
		#ipchains -A input -j ACCEPT -i $PPP_IFACE -p TCP  -d $PPP_NET ident
		#ipchains -A input -j ACCEPT -i $PPP_IFACE -p UDP  -d $PPP_NET 1024:
		#ipchains -A input -j ACCEPT -i $PPP_IFACE -p ICMP -d $PPP_NET
		#ipchains -A input -j DENY   -i $PPP_IFACE
		;;
    # add more ipppX cases if you need them...
esac