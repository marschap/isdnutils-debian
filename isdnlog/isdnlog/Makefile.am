AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=Makefile.in
INCLUDES=-I../tools -I../connect -I../../lib
DEFINES=-DBINDIR=${bindir} -DSBINDIR=${sbindir} -DI4LCONFDIR=@I4LCONFDIR@
sbin_PROGRAMS=isdnlog 
EXTRA_DIST=isdnlog.8 postgres.h postgres.c test_conter.c
isdnlog_SOURCES= functions.c isdnlog.c isdnlog.h processor.c server.c start_prog.c user_access.c
isdnlog_LDADD= -L`pwd`/../../lib -lisdn @ALINK@ -L../connect -lconnect -L../tools -ltools -L../misc -lmisc
