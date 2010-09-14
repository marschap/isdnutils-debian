#
# Makefile for Tcl/Tk-based tools
#
# (C) Fritz Elfert, 1995
#
#
#SCRIPTS=isdnmon isdntool
SCRIPTS=isdnmon

all:

isdnmon : isdnmon.tcl
	-doinst.sh isdnmon

isdntool : isdntool.tcl
	-doinst.sh isdntool

install: $(SCRIPTS)
	-install -o root -g bin -m 0750 $(SCRIPTS) /sbin

clean:
	rm -f *~ $(SCRIPTS)

