#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <streams.h>
#include <tap.h>

int test_num;

void tap_init(int total) {
    test_num = 1;
    fprintf(get_tap_log(), "1..%d\n", total);
}

void tap_ok(const char *comment, ...) {
    if (comment) {
        FILE *log = get_tap_log();
        fprintf(log, "ok %d - ", test_num++);
        va_list args;
        va_start(args, comment);
        vprintf(comment, args);
        va_end(args);
        putc('\n', log);
    } else {
        fprintf(get_tap_log(), "ok %d\n", test_num++);
    }
}

void tap_not_ok(const char *comment, ...) {
    if (comment) {
        FILE *log = get_tap_log();
        fprintf(log, "not ok %d - ", test_num++);
        va_list args;
        va_start(args, comment);
        vprintf(comment, args);
        va_end(args);
        putc('\n', log);
    } else {
        fprintf(get_tap_log(), "not ok %d\n", test_num++);
    }
}

void tap_fail(int exit_code, const char *comment, ...) {
    FILE *log = get_tap_log();
    va_list args;
    va_start(args, comment);
    vfprintf(log, comment, args);
    va_end(args);
    putc('\n', log);
    exit(exit_code);
}
