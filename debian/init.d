#! /bin/sh

#
# You don't need to change this file. Please read 
# /usr/doc/isdnutils/Readme.debian and /usr/doc/isdnutils/CONFIG.
#

PATH=/bin:/usr/bin:/sbin:/usr/sbin
NAME=isdnutils
FLAGS="defaults"
NOT_CONFIGED='not configured yet'

test -f /usr/sbin/iprofd || exit 0

case "$1" in
  start)
    echo -n "Starting isdn service daemons :"
    not_configured=''
    for devicefile in /etc/isdn/device.isdn? /etc/isdn/device.ippp? ; do
        test -e $devicefile || continue;
        device=${devicefile#*.}
        if grep -s "$NOT_CONFIGED" $devicefile > /dev/null
        then
	    not_configured="$not_configured$devicefile "
	else
            echo -n " if($device)"
            test -f $devicefile && sh ${devicefile} start > /dev/null
        fi
    done

    for optionfile in /etc/isdn/ipppd.ippp?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        if grep -s "$NOT_CONFIGED" $optionfile > /dev/null
        then
	    not_configured="$not_configured$optionfile "
	else
            echo -n " ipppd($device)" 
            start-stop-daemon --start --quiet --exec \
                /usr/sbin/ipppd -- file $optionfile /dev/$device &
        fi
    done

    if test -e /etc/isdn/iprofd.data ; then
	echo -n " iprofd" 
	start-stop-daemon --start --quiet \
	    --exec /usr/sbin/iprofd -- /etc/isdn/iprofd.data &
    fi

    for optionfile in /etc/isdn/isdnlog.isdnctrl?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        device=${device%.*}
        cardid=line`expr ${device#*l} / 2`
        if grep -s "$NOT_CONFIGED" $optionfile > /dev/null
        then
	    not_configured="$not_configured$optionfile "
	else
            echo -n " isdnlog($device)" 
	    # Enable debugging for this card (isdnlog requires "1 4",
	    # for D-Channel Q.931, call control messages).
	    hisaxctrl $cardid 1 4
            start-stop-daemon --start --quiet --exec \
                /usr/sbin/isdnlog -- -f$optionfile /dev/$device &
        fi
    done

    echo  "."

    if [ -n "$not_configured" ]
    then
	echo "Note: the following configuration files are not configured yet:"
	for i in $not_configured; do echo "	$i"; done
    fi
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

    for devicefile in /etc/isdn/device.isdn? /etc/isdn/device.ippp?; do
        test -e $devicefile || continue;
        device=${devicefile#*.}
        test -f $devicefile && sh ${devicefile} stop > /dev/null
        echo -n " if($device)"
    done

    for optionfile in /etc/isdn/isdnlog.isdnctrl?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        echo -n " isdnlog($device)" 
        start-stop-daemon --stop --quiet --exec /usr/sbin/isdnlog 
    done

    echo "."
    ;;
  *)
    echo "Usage: /etc/init.d/$NAME {start|stop}"
    exit 1
    ;;
esac

exit 0

