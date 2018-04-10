AC_INIT([zenith-bootstrap], [1.0], [zachdeibert@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_PROG_CC
AM_PROG_AR
AC_PROG_LIBTOOL

AC_DEFINE_UNQUOTED([SRC_DIR], ["`readlink -f $srcdir`"], [Define directory to the source code.])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    test/arithmetic/Makefile
    test/functions/Makefile
    test/greeter/Makefile
    test/hello-world/Makefile
    test/lib/Makefile
    test/Makefile
])
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_OUTPUT
