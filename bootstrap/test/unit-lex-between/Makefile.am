AM_CPPFLAGS = -I"$(srcdir)/../lib" -I "$(srcdir)/../../src"
check_PROGRAMS = unit-lex-between
unit_lex_between_LDFLAGS = -static
unit_lex_between_SOURCES = \
    main.c
unit_lex_between_LDADD = \
    ../../src/libzenith-bootstrap.la \
    ../lib/libzenith-bootstrap-tests.la

../../src/libzenith-bootstrap.la:
	$(MAKE) -C ../../src libzenith-bootstrap.la

../lib/libzenith-bootstrap-tests.la:
	$(MAKE) -C ../lib libzenith-bootstrap-tests.la
