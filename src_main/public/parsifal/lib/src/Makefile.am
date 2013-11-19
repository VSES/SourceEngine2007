lib_LTLIBRARIES = libparsifal.la
CSOURCES = bistream.c \
	encoding.c \
	xmlhash.c \
	parsifal.c \
	xmlsbuf.c \
	xmlvect.c \
	xmlpool.c \
	dtdvalid.c

INCLUDES=-I$(top_srcdir)
libparsifal_la_SOURCES = $(CSOURCES)

## RELEASE gets AC_SUBST'ed value from configure.ac:
libparsifal_la_LDFLAGS = -no-undefined -release @RELEASE@

## target include directory (/usr/include by default):
library_includedir=$(includedir)/libparsifal

## include_HEADERS is specified in /include/libparsifal/Makefile.am
## so we don't use include here:
## library_include_HEADERS = $(PUBHEADERS)

## flags for compiler:
AM_CFLAGS=-Wall -O3
## flags for preprocessor:
AM_CPPFLAGS=-I$(top_srcdir)/include/libparsifal -DDTD_SUPPORT -DMAX_SPEED -DDTDVALID_SUPPORT
