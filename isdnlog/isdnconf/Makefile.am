AUTOMAKE_OPTIONS=foreign no-dependencies
bin_PROGRAMS=isdnconf
man_MANS=isdnconf.8
MAINTAINERCLEANFILES=Makefile.in 
INCLUDES=-I../tools -I../../lib
DEFINES=-DBINDIR=${bindir} -DSBINDIR=${sbindir} -DI4LCONFDIR=@I4LCONFDIR@
isdnconf_SOURCES=isdnconf.c isdnconf.h 
isdnconf_LDADD=-L`pwd`/../../lib -lisdn @ALINK@ -L../tools -ltools 
