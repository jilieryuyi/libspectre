SUBDIRS = libspectre test

EXTRA_DIST = libspectre.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libspectre.pc

DISTCLEANFILES = $(pkgconfig_DATA)

dist-hook: ChangeLog

ChangeLog: .git/HEAD
	git log --stat > ChangeLog