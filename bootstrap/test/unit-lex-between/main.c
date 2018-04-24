#include <errno.h>
#include <string.h>
#include <config.h>
#include <lex.h>
#include <tap.h>

int main(int argc, const char **argv) {
    tap_init(10);
    lex_context_t context;
    lex_token_t token[46];
    char buffer[4096];
    if (lex_open(SRC_DIR "/test/hello-world/hello-world.zen", &context) < 0) {
        tap_fail(errno, "Failed to open file: %s", strerror(errno));
    }
    for (int i = 0; i < 46; ++i) {
        if (lex_next_token(&context, token + i) < 0) {
            tap_fail(errno, "Failed to read token: %s", strerror(errno));
        }
    }
#define TEST(from, to, expected) \
    if (lex_read_between(token + from, token + to, buffer, 4096) < 0) { \
        tap_fail(errno, "Failed to read between tokens: %s", strerror(errno)); \
    } \
    if (strcmp(expected, buffer) == 0) { \
        tap_ok(NULL); \
    } else { \
        tap_not_ok("Expected '%s', but got '%s'.", expected, buffer); \
    }
    TEST(3, 35, " << \"Hello, world!\n\"\n$msg_end\n\n@exec@\n\n$write\n    + 1\n    + 4\n    syscall\n    --\n    ret\n\n$exit\n    + 1\n    syscall\n\n$main\n    + ");
    TEST(5, 36, " \"Hello, world!\n\"\n$msg_end\n\n@exec@\n\n$write\n    + 1\n    + 4\n    syscall\n    --\n    ret\n\n$exit\n    + 1\n    syscall\n\n$main\n    + msg\n    ");
    TEST(14, 26, "exec@\n\n$write\n    + 1\n    + 4\n    syscall\n    --\n    ");
    TEST(16, 20, "\n\n$write\n    + ");
    TEST(8, 39, " world!\n\"\n$msg_end\n\n@exec@\n\n$write\n    + 1\n    + 4\n    syscall\n    --\n    ret\n\n$exit\n    + 1\n    syscall\n\n$main\n    + msg\n    + msg_end\n    sub\n    ");
    TEST(33, 34, "\n    ");
    TEST(18, 19, "\n    ");
    TEST(14, 38, "exec@\n\n$write\n    + 1\n    + 4\n    syscall\n    --\n    ret\n\n$exit\n    + 1\n    syscall\n\n$main\n    + msg\n    + msg_end\n    ");
    TEST(2, 13, "msg << \"Hello, world!\n\"\n$");
    TEST(28, 32, "\n    + 1\n    syscall\n\n");
    if (lex_close(&context) < 0) {
        tap_fail(errno, "Failed to close file: %s", strerror(errno));
    }
    return 0;
}
