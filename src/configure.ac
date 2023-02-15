#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pfrd], [0.01], [mityagz@gmail.com])
AC_CONFIG_SRCDIR([pfr_peers.cpp])
AC_CONFIG_HEADERS([config.h])

: ${CXXFLAGS=-g -Werror -O0}

AC_CANONICAL_HOST

# Checks for programs.
case "${host_os}" in
    linux*)
        AC_PROG_CXX
        AC_PROG_CC
        AC_PROG_CPP
        ;;
    freebsd*)
        AC_PROG_CC(clang llvm-gcc gcc)
        AC_PROG_CXX(clang++ llvm-g++ g++)
        AC_PROG_CPP
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac


AM_INIT_AUTOMAKE

# Checks for libraries.
#PKG_CHECK_MODULES([GLIB], [glib-2.0])

CFLAGS="$CFLAGS -I/usr/local/include"
CPPFLAGS="$CPPFLAGS -I/usr/local/include"
LDFLAGS="$LDFLAGS -L/usr/local/lib"

# Checks for libraries.
AC_CHECK_LIB([crypt], [main])
AC_CHECK_LIB([pq], [main])
AC_CHECK_HEADER([glib.h])
#PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.28])
#AC_CHECK_LIB([glib], [main])
AC_CHECK_LIB([pthread], [main])
AC_CHECK_LIB([netconf], [main])
AC_CHECK_LIB([spdlog], [main])
AC_CHECK_LIB([boost_system], [main])
AC_CHECK_LIB([boost_program_options], [main])
AC_CHECK_LIB([boost_regex], [main])


##
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/foreach.hpp], [],[AC_MSG_ERROR(You need the Boost libraries.)])
AC_LANG_POP([C++])
##

#AX_BOOST_BASE([MINIMUM-VERSION], [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#AX_BOOST_BASE([1.0], )

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
