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

dnl Test
AC_ARG_ENABLE(test,
              [AC_HELP_STRING([--enable-test], [Compile tests])],enable_test="$enableval",enable_test=yes)

if test "x$enable_test" = "xyes"; then
    PKG_CHECK_MODULES(CAIRO, cairo cairo-png, enable_test=yes,enable_test=no)

    if test "x$enable_test" != "xyes"; then
        AC_MSG_WARN("Tests disabled since required cairo library was not found")
    fi
fi

AM_CONDITIONAL(ENABLE_TEST, test x$enable_test = xyes)

dnl ===========================================================================
dnl check compiler flags (From cairo)
AC_DEFUN([SPECTRE_CC_TRY_FLAG], [
  AC_MSG_CHECKING([whether $CC supports $1])

  spectre_save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS $1"

  AC_COMPILE_IFELSE([ ], [spectre_cc_flag=yes], [spectre_cc_flag=no])
  CFLAGS="$spectre_save_CFLAGS"

  if test "x$spectre_cc_flag" = "xyes"; then
    ifelse([$2], , :, [$2])
  else
    ifelse([$3], , :, [$3])
  fi
  AC_MSG_RESULT([$spectre_cc_flag])
])


dnl Use lots of warning flags with with gcc and compatible compilers

dnl Note: if you change the following variable, the cache is automatically
dnl skipped and all flags rechecked.  So there's no need to do anything
dnl else.  If for any reason you need to force a recheck, just change
dnl MAYBE_WARN in an ignorable way (like adding whitespace)

MAYBE_WARN="-Wall -ansi \
-Wsign-compare -Werror-implicit-function-declaration \
-Wpointer-arith -Wstrict-prototypes \
-Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
-Wpacked -Wswitch-enum \
-Wstrict-aliasing=2 -Winit-self -Wunsafe-loop-optimizations \
-Wdeclaration-after-statement \
-Wno-missing-field-initializers -Wno-unused-parameter \
-Wno-attributes -Wno-long-long -Winline"


# invalidate cached value if MAYBE_WARN has changed
if test "x$spectre_cv_warn_maybe" != "x$MAYBE_WARN"; then
	unset spectre_cv_warn_cflags
fi
AC_CACHE_CHECK([for supported warning flags], spectre_cv_warn_cflags, [
	echo
	WARN_CFLAGS=""

	# Some warning options are not supported by all versions of
	# gcc, so test all desired options against the current
	# compiler.
	#
	# Note that there are some order dependencies
	# here. Specifically, an option that disables a warning will
	# have no net effect if a later option then enables that
	# warnings, (perhaps implicitly). So we put some grouped
	# options (-Wall and -Wextra) up front and the -Wno options
	# last.

	for W in $MAYBE_WARN; do
		SPECTRE_CC_TRY_FLAG([$W], [WARN_CFLAGS="$WARN_CFLAGS $W"])
	done

	spectre_cv_warn_cflags=$WARN_CFLAGS
	spectre_cv_warn_maybe=$MAYBE_WARN

	AC_MSG_CHECKING([which warning flags were supported])])
WARN_CFLAGS="$spectre_cv_warn_cflags"
SPECTRE_CFLAGS="$SPECTRE_CFLAGS $WARN_CFLAGS"

AC_SUBST(SPECTRE_CFLAGS)

AC_OUTPUT([
Makefile
libspectre/Makefile
libspectre/ghostscript/Makefile
test/Makefile
libspectre.pc
])
