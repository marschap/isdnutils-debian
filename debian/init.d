#! /bin/sh

#
# You don't need to change this file. Please read 
# /usr/doc/isdnutils/Readme.debian and /usr/doc/isdnutils/CONFIG.
#

PATH=/bin:/usr/bin:/sbin:/usr/sbin
NAME=isdnutils
FLAGS="defaults"

test -f /usr/sbin/iprofd || exit 0

case "$1" in
  start)
    echo -n "Starting isdn service daemons :"
    for devicefile in /etc/isdn/device.isdn? /etc/isdn/device.ippp? ; do
	test -e $devicefile || continue;
        device=${devicefile#*.}
	test -f $devicefile && sh ${devicefile} start > /dev/null
        echo -n " device($device)"
    done

    if test -e /etc/isdn/iprofd.data ; then
    	echo -n " iprofd" 
        start-stop-daemon --start --quiet \
            --exec /usr/sbin/iprofd -- /etc/isdn/iprofd.data &
    fi

    for optionfile in /etc/isdn/ipppd.ippp?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        echo -n " ipppd($device)" 
        start-stop-daemon --start --quiet --exec \
            /usr/sbin/ipppd -- file $optionfile /dev/$device &
    done

    for optionfile in /etc/isdn/isdnlog.isdnctrl?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        device=${device%.*}
        echo -n " isdnlog($device)" 
        start-stop-daemon --start --quiet --exec \
            /usr/sbin/isdnlog -- -f$optionfile /dev/$device &
    done
    echo  "."
    ;;

  stop)
    echo -n "Stopping isdn service daemons :"
    if test -n "`pidof iprofd`"; then
    	echo -n " iprofd" 
	start-stop-daemon --stop --quiet --exec /usr/sbin/iprofd
    fi

    for optionfile in /etc/isdn/ipppd.ippp?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        echo -n " ipppd($device)" 
        start-stop-daemon --stop --quiet --exec /usr/sbin/ipppd
    done

    for optionfile in /etc/isdn/isdnlog.isdnctrl?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        echo -n " isdnlog($device)" 
        start-stop-daemon --stop --quiet --exec /usr/sbin/isdnlog 
    done
    for devicefile in /etc/isdn/device.isdn? /etc/isdn/device.ippp?; do
        test -e $devicefile || continue;
        device=${devicefile#*.}
	test -f $devicefile && sh ${devicefile} stop > /dev/null
        echo -n " device($device)"
    done
    echo "."
    ;;
  *)
    echo "Usage: /etc/init.d/$NAME {start|stop}"
    exit 1
    ;;
esac

exit 0

