#!/bin/sh
#
# You don't need to change this file!
#
# Please read /usr/share/doc/isdnutils/README.debian
# and         /usr/share/doc/isdnutils/CONFIG.gz .
#
# This file is for stopping and starting isdnlog only.
# The other ISDN stuff is handled via /etc/init.d/isdnutils .

NAME=isdnlog
PATH=/bin:/usr/bin:/sbin:/usr/sbin
FLAGS="defaults"

test -f /usr/sbin/isdnlog || exit 0    # sanity check

# source the function definitions for stopping / starting the various parts
# (also used in /etc/init.d/isdnutils)

. /etc/isdn/init.d.functions


quiet_start=false


case "$1" in
  start)
    echo -n "Starting isdnlog :"
    start_isdnlog
    ;;
  stop)
    echo -n "Stopping isdnlog :"
    stop_isdnlog
    ;;
  reload)
    echo -n "Reloading isdnlog :"
    quiet_start=true
    reload_isdnlog
    ;;
  restart|force-reload)
    # reload doesn't really work for these daemons (except isdnlog)...
    echo -n "Restarting isdnlog :"
    quiet_start=true
    stop_isdnlog; start_isdnlog
    ;;
  *)
    echo "Usage: /etc/init.d/$NAME {start|stop|reload|restart|force-reload}"
    exit 1
    ;;
esac

echo "."
list_unconfigured

exit 0
# vim:set ts=8:
