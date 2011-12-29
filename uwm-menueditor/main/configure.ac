AC_PREREQ([2.68])
AC_INIT([uwm-menued], [0.1], [knightLA@yandex.ru])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([menued/main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([X11], [XOpenDisplay])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([gui/Makefile menued/Makefile Makefile])

AC_OUTPUT
