#!/bin/sh

if [ "`id | grep uid=0`" = "" ]; then
	echo "In order to create device inodes, you must run this script"
	echo "as root."
	exit 1
fi
echo "Creating device inodes ..."

DEV=/dev

MAJ=45
MIN=0
rm -f $DEV/isdnctrl* $DEV/ippp*
while [ $MIN -lt 64 ] ; do
#	mknod -m 600 $DEV/isdn$MIN c $MAJ $MIN
	mknod -m 600 $DEV/isdnctrl$MIN c $MAJ `expr $MIN + 64`
	mknod -m 600 $DEV/ippp$MIN c $MAJ `expr $MIN + 128`
	MIN=`expr $MIN + 1`
done
rm -f $DEV/isdninfo
mknod -m 444 $DEV/isdninfo c $MAJ 255
ln -sf $DEV/isdnctrl0 $DEV/isdnctrl

MAJ=43
MIN=0
rm -f $DEV/ttyI*
while [ $MIN -lt 64 ] ; do
	mknod -m 666 $DEV/ttyI$MIN c $MAJ $MIN
	MIN=`expr $MIN + 1`
done

MAJ=44
MIN=0
rm -f $DEV/cui*
while [ $MIN -lt 64 ] ; do
	mknod -m 666 $DEV/cui$MIN c $MAJ $MIN
	MIN=`expr $MIN + 1`
done
