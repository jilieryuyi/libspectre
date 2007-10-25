INCLUDES = 			\
	-I$(top_builddir)	\
	-I$(srcdir)		\
	-I$(top_srcdir)

lib_LTLIBRARIES = libspectre.la

libspectreincludedir = $(includedir)/libscpectre
libspectreinclude_HEADERS =		\
	spectre.h			\
	spectre-macros.h		\
	spectre-status.h		\
	spectre-document.h		\
	spectre-page.h

libspectre_la_SOURCES = 		\
	spectre.h			\
	spectre-private.h		\
	spectre-macros.h		\
	spectre-status.h		\
	spectre-status.c		\
	spectre-document.h		\
	spectre-document.c		\
	spectre-page.h			\
	spectre-page.c

libspectre_la_LIBADD = $(LIB_GS)
