dnl Process this file with autoconf to produce a configure script.

AC_INIT(capi20.c)
AC_ARG_ENABLE([V2],
  AS_HELP_STRING([--enable-V2],[build libcapi20 version 2 compatible library]),
     [case "${enableval}" in
       yes) LIBCAPI_VERSION=2
	    LIBCAPI_VERSION_AGE=0
	    LIBCAPI_VERSION_RELEASE=12
	    AC_DEFINE([CAPI_LIBRARY_V2],1)
	    ;;
       no) LIBCAPI_VERSION=3
	    LIBCAPI_VERSION_AGE=0
	    LIBCAPI_VERSION_RELEASE=6
	    ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-V2) ;;
     esac],
     [
        LIBCAPI_VERSION=3
	LIBCAPI_VERSION_AGE=0
	LIBCAPI_VERSION_RELEASE=6
])

AM_INIT_AUTOMAKE(libcapi20,${LIBCAPI_VERSION}.${LIBCAPI_VERSION_RELEASE})
AC_PREFIX_DEFAULT(/usr)
AC_SUBST(BUILD)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
AC_OBJEXT
AC_PATH_PROG(SED,sed)

AC_LIBTOOL_WIN32_DLL

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h unistd.h)

AC_COMPUTE_INT([CAPI_MODULE_LOADER_VERSION], [[MODULE_LOADER_VERSION]],
	[
		#include <sys/types.h>
		#include "capi_mod.h"
	],
	[AC_MSG_ERROR([capi library MODULE_LOADER_VERSION cannot be calculated])]
)
AC_MSG_RESULT([Capi library module loader version ${CAPI_MODULE_LOADER_VERSION} detected]) 
AC_SUBST(CAPI_MODULE_LOADER_VERSION)

dnl Checks for library functions.
AC_CHECK_FUNCS(select)
AC_SEARCH_LIBS([dlopen], dl)

dnl check OS
AC_CANONICAL_HOST
case "$host" in
  *linux*)
    AC_CHECK_HEADERS(linux/capi.h,,AC_MSG_ERROR( Required header (linux/capi.h) missing ))
    kernel_capi=true
    win32=false
    ;;
  *-*-*mingw*)
    kernel_capi=false
    win32=true
    ;;
  *)
    kernel_capi=false
    win32=false
    ;;
esac
AM_CONDITIONAL(WIN32, test "x$win32" = xtrue)
AC_SUBST(WIN32)

AM_CONDITIONAL(KERNEL_CAPI, test x$kernel_capi = xtrue)
AC_SUBST(KERNEL_CAPI)

dnl Check for required types
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_CHECK_TYPE([ulong], [AC_DEFINE([C_ULONG_DEFINED], [1], [Define to 1 if the ulong type exists in C.])])
AC_CHECK_SIZEOF(void *)

AC_SUBST(INSTALL)
AC_SUBST(LIBCAPI_VERSION)
AC_SUBST(LIBCAPI_VERSION_AGE)
AC_SUBST(LIBCAPI_VERSION_RELEASE)
AC_OUTPUT(Makefile capi20.pc)
