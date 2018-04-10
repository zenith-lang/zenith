bin_PROGRAMS = zenith-bootstrap
zenith_bootstrap_SOURCES = main.c
zenith_bootstrap_LDADD = libzenith-bootstrap.la
zenith_bootstrap_LDFLAGS = -static
lib_LTLIBRARIES = libzenith-bootstrap.la
include_HEADERS = \
    entry.h \
    interpreter.h \
    parser.h \
    reader.h \
    stack.h \
    syscall.h
libzenith_bootstrap_la_SOURCES = \
    entry.c \
    interpreter.c \
    parser.c \
    reader.c \
    stack.c \
    syscall.c

syscall.o: syscall/independent/1.c syscall/independent/3.c syscall/independent/4.c
