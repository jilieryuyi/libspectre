AC_PREREQ(2.59)
AC_INIT(libspectre, 0.0.1)
AC_CONFIG_HEADERS(config.h)

AM_INIT_AUTOMAKE([1.7])

AC_PROG_LIBTOOL
AC_PROG_CC
AC_ISC_POSIX
AC_PROG_CC_STDC
AC_STDC_HEADERS

AC_CHECK_LIB(gs, gsapi_new_instance, have_libgs=yes, have_libgs=no)
if test "x$have_libgs" = "xyes"; then
    LIB_GS="-lgs"
else
    AC_MSG_ERROR([You need libgs in order to compile libspectre])
fi
AC_SUBST(LIB_GS)

AC_OUTPUT([
Makefile
libspectre/Makefile
test/Makefile
libspectre.pc
])
