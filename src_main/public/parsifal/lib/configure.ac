AC_INIT(parsifal, 0.9.3)
AC_CONFIG_AUX_DIR(conf)
AC_CONFIG_SRCDIR(src/parsifal.c)

## to avoid default install dir /usr/local and
## LD_LIBRARY_PATH etc hassle:
AC_PREFIX_DEFAULT(/usr)

## PACKAGE and VERSION vars will get their values from AM_INIT_AUTOMAKE:
AM_INIT_AUTOMAKE(libparsifal, 0.9.3)

RELEASE="0.9.3"
AC_SUBST([RELEASE])

## Checks for programs:
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL

AC_LANG_C
AM_SANITY_CHECK

AC_OUTPUT( Makefile \
	src/Makefile \
	include/Makefile \
	include/libparsifal/Makefile \
)

echo
echo "==============================="
echo "libparsifal $VERSION"
echo "install prefix: $prefix"
echo "==============================="


