AUTOMAKE_OPTIONS=foreign no-dependencies
MAINTAINERCLEANFILES=Makefile.in
INCLUDES=-I../tools -I../../lib
DEFINES=-DBINDIR=${bindir} -DSBINDIR=${sbindir} -DI4LCONFDIR=@I4LCONFDIR@
noinst_LIBRARIES=libconnect.a
libconnect_a_SOURCES= connect.c conv_address.c socket.c socket.h
