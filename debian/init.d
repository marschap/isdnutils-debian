#! /bin/sh

#
# You don't need to change this file. Please read 
# /usr/doc/isdnutils/README.debian and /usr/doc/isdnutils/CONFIG.gz .
#

PATH=/bin:/usr/bin:/sbin:/usr/sbin
NAME=isdnutils
FLAGS="defaults"
NOT_CONFIGED='not configured yet'

test -f /usr/sbin/iprofd || exit 0	# sanity check

##########################
# start and stop functions
##########################

start_isdnlog() {
    what=' isdnlog'
    for optionfile in /etc/isdn/isdnlog.isdnctrl?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        device=${device%.*}
        cardid=line`expr ${device#*l} / 2`
        if grep -s "$NOT_CONFIGED" $optionfile > /dev/null
        then
	    not_configured="$not_configured$optionfile "
	else
	    $quiet_start || echo -n "$what"; what=''
	    # Enable debugging for this card (isdnlog requires "1 4",
	    # for D-Channel Q.931, call control messages).
	    hisaxctrl $cardid 1 4
            start-stop-daemon --start --quiet --exec \
                /usr/sbin/isdnlog -- -f$optionfile /dev/$device &
        fi
    done
}

stop_isdnlog() {
    what=' isdnlog'
    for optionfile in /etc/isdn/isdnlog.isdnctrl?; do
        test -e $optionfile || continue;
	echo -n "$what"; what=''
        device=${optionfile#*.}
        start-stop-daemon --stop --quiet --exec /usr/sbin/isdnlog 
	break	# only need to do start-stop-daemon once
    done
}

start_devices() {
    what=' interfaces'
    for devicefile in /etc/isdn/device.isdn? /etc/isdn/device.ippp? ; do
        test -e $devicefile || continue;
        device=${devicefile#*.}
        if grep -s "$NOT_CONFIGED" $devicefile > /dev/null
        then
	    not_configured="$not_configured$devicefile "
	else
            if test -f $devicefile; then
		$quiet_start || echo -n "$what"; what=''
		sh ${devicefile} start > /dev/null
	    fi
        fi
    done
}

stop_devices() {
    what=' interfaces'
    for devicefile in /etc/isdn/device.isdn? /etc/isdn/device.ippp?; do
        test -e $devicefile || continue;
        device=${devicefile#*.}
        if test -f $devicefile; then
	    echo -n "$what"; what=''
	    sh ${devicefile} stop > /dev/null
	fi
    done
}

start_ipppd() {
    what=' ipppd'
    for optionfile in /etc/isdn/ipppd.ippp?; do
        test -e $optionfile || continue;
        device=${optionfile#*.}
        if grep -s "$NOT_CONFIGED" $optionfile > /dev/null
        then
	    not_configured="$not_configured$optionfile "
	else
	    $quiet_start || echo -n "$what"; what=''
            start-stop-daemon --start --quiet \
		--pidfile /var/run/ipppd.$device.pid \
		--exec /usr/sbin/ipppd -- file $optionfile /dev/$device &
        fi
    done
}

stop_ipppd() {
    what=' ipppd'
    for optionfile in /etc/isdn/ipppd.ippp?; do
        test -e $optionfile || continue;
	echo -n "$what"; what=''
        device=${optionfile#*.}
        start-stop-daemon --stop --quiet \
		--pidfile /var/run/ipppd.$device.pid \
		--exec /usr/sbin/ipppd
    done
}

start_iprofd() {
    if test -e /etc/isdn/iprofd.data ; then
	$quiet_start || echo -n " iprofd" 
	start-stop-daemon --start --quiet \
	    --exec /usr/sbin/iprofd -- /etc/isdn/iprofd.data &
    fi
}

stop_iprofd() {
    if test -n "`pidof iprofd`"; then
    	echo -n " iprofd" 
	start-stop-daemon --stop --quiet --exec /usr/sbin/iprofd
    fi
}


##########################
# Start of option handling
##########################

not_configured=''
quiet_start=false

case "$1" in
  start)
    echo -n "Starting isdn services :"
    start_isdnlog
    start_devices
    start_ipppd
    start_iprofd
    echo  "."
    if [ -n "$not_configured" ]
    then
	echo "Note: the following configuration files are not configured yet:"
	for i in $not_configured; do echo "	$i"; done
    fi
    ;;

  stop)
    echo -n "Stopping isdn services :"
    stop_iprofd
    stop_ipppd
    stop_devices
    stop_isdnlog
    echo "."
    ;;

  restart|force-reload)
    # reload doesn't really work for these daemons...
    echo -n "Restarting isdn services :"
    quiet_start=true
    stop_isdnlog; start_isdnlog
    stop_iprofd;  start_iprofd
    stop_ipppd
    stop_devices; start_devices
    start_ipppd
    echo "."
    if [ -n "$not_configured" ]
    then
	echo "Note: the following configuration files are not configured yet:"
	for i in $not_configured; do echo "	$i"; done
    fi
    ;;

  *)
    echo "Usage: /etc/init.d/$NAME {start|stop|restart|force-reload}"
    exit 1
    ;;
esac

exit 0

