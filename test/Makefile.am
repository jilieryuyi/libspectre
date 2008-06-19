INCLUDES = 				\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/libspectre	\
	$(SPECTRE_CFLAGS)		\
	$(CAIRO_CFLAGS)

noinst_PROGRAMS = spectre-test parser-test

spectre_test_SOURCES = spectre-test.c
spectre_test_LDADD =                         		\
	$(top_builddir)/libspectre/libspectre.la	\
	$(CAIRO_LIBS)

parser_test_SOURCES = parser-test.c
parser_test_LDADD = $(top_builddir)/libspectre/libspectre.la
