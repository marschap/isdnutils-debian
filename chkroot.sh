#!/bin/sh

if [ "`id | grep uid=0`" = "" ]; then
	echo "In order to $* the package, you must run this script"
	echo "as root."
	exit 1
fi
