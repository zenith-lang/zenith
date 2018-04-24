AM_CPPFLAGS = -I"$(srcdir)/../lib" -I "$(srcdir)/../../src"
check_PROGRAMS = unit-read-line
unit_read_line_LDFLAGS = -static
unit_read_line_SOURCES = \
    main.c
unit_read_line_LDADD = \
    ../../src/libzenith-bootstrap.la \
    ../lib/libzenith-bootstrap-tests.la

../../src/libzenith-bootstrap.la:
	$(MAKE) -C ../../src libzenith-bootstrap.la

../lib/libzenith-bootstrap-tests.la:
	$(MAKE) -C ../lib libzenith-bootstrap-tests.la
