AM_CPPFLAGS = -I"$(srcdir)/../lib" -I "$(srcdir)/../../src"
check_PROGRAMS = hello-world
hello_world_SOURCES = \
    main.c
hello_world_LDADD = \
    ../../src/libzenith-bootstrap.la \
    ../lib/libzenith-bootstrap-tests.la

../../src/libzenith-bootstrap.la:
	$(MAKE) -C ../../src libzenith-bootstrap.la

../lib/libzenith-bootstrap-tests.la:
	$(MAKE) -C ../lib libzenith-bootstrap-tests.la
