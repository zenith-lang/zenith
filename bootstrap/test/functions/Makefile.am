AM_CPPFLAGS = -I"$(srcdir)/../lib" -I "$(srcdir)/../../src"
check_PROGRAMS = functions
functions_SOURCES = \
    main.c
functions_LDADD = \
    ../../src/libzenith-bootstrap.la \
    ../lib/libzenith-bootstrap-tests.la

../../src/libzenith-bootstrap.la:
	$(MAKE) -C ../../src libzenith-bootstrap.la

../lib/libzenith-bootstrap-tests.la:
	$(MAKE) -C ../lib libzenith-bootstrap-tests.la
