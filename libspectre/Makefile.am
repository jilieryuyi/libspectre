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

gv_sources = 				\
	ps.h				\
	ps.c

libspectre_la_SOURCES = 		\
	spectre.h			\
	spectre-private.h		\
	spectre-macros.h		\
	spectre-status.h		\
	spectre-status.c		\
	spectre-document.h		\
	spectre-document.c		\
	spectre-device.h		\
	spectre-device.c		\
	spectre-page.h			\
	spectre-page.c			\
	$(gv_sources)

libspectre_la_LIBADD = $(LIB_GS)
