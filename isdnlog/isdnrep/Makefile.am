AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=Makefile.in 
INCLUDES=-I../tools -I../../lib -I../isdnlog -I../connect
DEFINES=-DBINDIR=${bindir} -DSBINDIR=${sbindir} -DI4LCONFDIR=@I4LCONFDIR@
bin_PROGRAMS=isdnrep 
isdnrep_SOURCES=rep_main.c isdnrep.c isdnrep.h
isdnrep_LDADD=-L`pwd`/../../lib -lisdn @ALINK@ -L../tools -ltools -L../misc -lmisc
