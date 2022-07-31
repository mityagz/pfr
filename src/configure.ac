#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pfrd], [0.01], [mityagz@gmail.com])
AC_CONFIG_SRCDIR([pfr_peers.cpp])
AC_CONFIG_HEADERS([config.h])

: ${CXXFLAGS=-g -Werror -O0}

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

AM_INIT_AUTOMAKE

# Checks for libraries.
#PKG_CHECK_MODULES([GLIB], [glib-2.0])


# Checks for libraries.
AC_CHECK_LIB([crypt], [main])
AC_CHECK_LIB([pq], [main])
AC_CHECK_HEADER([glib.h])
#PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.28])
#AC_CHECK_LIB([glib], [main])
AC_CHECK_LIB([pthread], [main])
AC_CHECK_LIB([netconf], [main])


CFLAGS="$CFLAGS $GLIB_CFLAGS -std=gnu99 $(echo '${UFLAGS}')"
CPPFLAGS="$CPPFLAGS $GLIB_CPPFLAG"
LDFLAGS="$LDFLAGS $GLIB_LDFLAGS"


# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h malloc.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([alarm bzero dup2 gethostname gettimeofday memset select socket strdup strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
