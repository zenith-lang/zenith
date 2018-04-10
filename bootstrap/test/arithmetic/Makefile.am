AM_CPPFLAGS = -I"$(srcdir)/../lib" -I "$(srcdir)/../../src"
check_PROGRAMS = arithmetic
arithmetic_SOURCES = \
    main.c
arithmetic_LDADD = \
    ../../src/libzenith-bootstrap.la \
    ../lib/libzenith-bootstrap-tests.la

../../src/libzenith-bootstrap.la:
	$(MAKE) -C ../../src libzenith-bootstrap.la

../lib/libzenith-bootstrap-tests.la:
	$(MAKE) -C ../lib libzenith-bootstrap-tests.la
