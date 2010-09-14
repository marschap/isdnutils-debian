AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES= Makefile.in 
INCLUDES=-I../tools -I../../lib
DEFINES=-DBINDIR=${bindir} -DSBINDIR=${sbindir} -DI4LCONFDIR=@I4LCONFDIR@
bin_PROGRAMS=isdnconf
isdnconf_SOURCES=isdnconf.c isdnconf.h 
isdnconf_LDADD=-L`pwd`/../../lib -lisdn @ALINK@ -L../tools -ltools 
