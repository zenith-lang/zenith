#include <errno.h>
#include <string.h>
#include <config.h>
#include <lex.h>
#include <tap.h>

int main(int argc, const char **argv) {
    tap_init(46);
    lex_context_t context;
    lex_token_t token;
    if (lex_open(SRC_DIR "/test/hello-world/hello-world.zen", &context) < 0) {
        tap_fail(errno, "Failed to open file: %s", strerror(errno));
    }
#define TEST(expected) \
    if (lex_next_token(&context, &token) < 0) { \
        tap_fail(errno, "Failed to read token: %s", strerror(errno)); \
    } \
    if (strcmp(token.value, expected) == 0) { \
        tap_ok(NULL); \
    } else { \
        tap_not_ok("Expected '%s', but got '%s'.", expected, token.value); \
    }
    TEST("@");
    TEST("@");
    TEST("$");
    TEST("msg");
    TEST("<");
    TEST("<");
    TEST("\"");
    TEST("Hello");
    TEST(",");
    TEST("world");
    TEST("!");
    TEST("\"");
    TEST("$");
    TEST("msg_end");
    TEST("@");
    TEST("exec");
    TEST("@");
    TEST("$");
    TEST("write");
    TEST("+");
    TEST("1");
    TEST("+");
    TEST("4");
    TEST("syscall");
    TEST("-");
    TEST("-");
    TEST("$");
    TEST("exit");
    TEST("+");
    TEST("1");
    TEST("syscall");
    TEST("$");
    TEST("main");
    TEST("+");
    TEST("msg");
    TEST("+");
    TEST("msg_end");
    TEST("sub");
    TEST("+");
    TEST("msg");
    TEST("+");
    TEST("write");
    TEST("call");
    TEST("+");
    TEST("exit");
    TEST("call");
    if (lex_close(&context) < 0) {
        tap_fail(errno, "Failed to close file: %s", strerror(errno));
    }
    return 0;
}
