#define ZENITH_LEX_C
#include <errno.h>
#include <string.h>
#include <config.h>
#include <lex.h>
#include <tap.h>

int main(int argc, const char **argv) {
    tap_init(31);
    lex_context_t context;
    if (lex_open(SRC_DIR "/test/hello-world/hello-world.zen", &context) < 0) {
        tap_fail(errno, "Failed to open file: %s", strerror(errno));
    }
#define TEST(expected) \
    if (strcmp(context.line, expected) == 0) { \
        tap_ok(NULL); \
    } else { \
        tap_not_ok("Expected '%s', but got '%s'.", expected, context.line); \
    } \
    if (lex_read_line(&context) < 0) { \
        tap_fail(errno, "Failed to read line: %s", strerror(errno)); \
    }
    TEST("#!/usr/bin/env zenith");
    TEST("");
    TEST("@@");
    TEST("");
    TEST("$msg << \"Hello, world!");
    TEST("\"");
    TEST("$msg_end");
    TEST("");
    TEST("@exec@");
    TEST("");
    TEST("$write");
    TEST("    + 1");
    TEST("    + 4");
    TEST("    syscall");
    TEST("    --");
    TEST("    ret");
    TEST("");
    TEST("$exit");
    TEST("    + 1");
    TEST("    syscall");
    TEST("");
    TEST("$main");
    TEST("    + msg");
    TEST("    + msg_end");
    TEST("    sub");
    TEST("    + msg");
    TEST("    + write");
    TEST("    call");
    TEST("    + exit");
    TEST("    call");
    TEST("");
    if (lex_close(&context) < 0) {
        tap_fail(errno, "Failed to close file: %s", strerror(errno));
    }
    return 0;
}
