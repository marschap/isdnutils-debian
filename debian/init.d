#!/bin/sh
#
# You don't need to change this file!
#
# Please read /usr/doc/isdnutils/README.debian
# and         /usr/doc/isdnutils/CONFIG.gz .
#
# This script stops/starts ALL ISDN stuff (including isdnlog, if installed).

NAME=isdnutils
PATH=/bin:/usr/bin:/sbin:/usr/sbin
FLAGS="defaults"

test -f /usr/sbin/isdnctrl || exit 0    # sanity check

# source the function definitions for stopping / starting the various parts
# (also used in /etc/init.d/isdnlog)

. /etc/isdn/init.d.functions

# isdn_verbose=true     # default is set by VERBOSE in /etc/default/rcS

case "$1" in
  start)
    echo -n "Starting isdn services :"
    start_devices
    start_ipppd
    start_iprofd
    if [ -s /usr/sbin/isdnlog ]; then start_isdnlog; fi
    ;;
  stop)
    echo -n "Stopping isdn services :"
    stop_iprofd
    stop_ipppd
    stop_devices
    if [ -s /usr/sbin/isdnlog ]; then stop_isdnlog; fi
    ;;
  restart|force-reload)
    # reload doesn't really work for these daemons...
    echo -n "Restarting isdn services :"
    isdn_verbose=false
    stop_iprofd
    stop_ipppd
    if [ -s /usr/sbin/isdnlog ]; then stop_isdnlog; start_isdnlog; fi
    stop_devices; start_devices
    start_ipppd
    start_iprofd
    ;;
  *)
    echo "Usage: /etc/init.d/$NAME {start|stop|restart|force-reload}"
    exit 1
    ;;
esac

echo "."
list_unconfigured

exit 0
# vim:set ts=8:
