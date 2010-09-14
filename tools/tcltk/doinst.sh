#!/bin/sh

if [ "$1" = "isdntool" ]; then
  LOOKFOR=tixwish
else
  LOOKFOR=wish
fi
WISH=
IFS=:
for i in $PATH; do
	if [ -x $i/$LOOKFOR ]; then
		WISH="$i/$LOOKFOR"
	fi
done
IFS=' '

while [ ".${WISH}" = "." ]; do
	echo ""
	echo 'In order to create isdnmon (a TclTk-script) you need an'
	echo "installed ${LOOKFOR}" '(the TclTk-interpreter), however it could'
	echo 'not be found in your current PATH. If you do not know where it'
	echo 'is, or you do not have X11, just enter "q" at the following'
	echo 'question.'
	echo -n "Enter full path of $LOOKFOR : "
	read WISH
done

if [ "`$WISH -f doinst.tcl`" = "1" ]; then
	cat $1.tcl | sed -e s@_WISH_@$WISH@ > $1
	chmod a+x $1
	install -o root -g bin -m 0750 $1 /sbin
else
	echo "UNABLE TO FIND WISH OR WRONG VERSION, isdnmon NOT INSTALLED"
	exit 1
fi
