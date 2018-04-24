SUBDIRS = \
    src \
    test
dist_doc_DATA = README
LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh
TESTS = \
    test/unit-read-line/unit-read-line \
    test/unit-lex/unit-lex \
    test/unit-lex-line/unit-lex-line \
    test/unit-lex-between/unit-lex-between \
    test/hello-world/hello-world
XFAIL_TESTS = \
    test/hello-world/hello-world
EXTRA_DIST = test/
ACLOCAL_AMFLAGS = -I m4
