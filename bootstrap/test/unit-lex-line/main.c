#include <errno.h>
#include <string.h>
#include <config.h>
#include <lex.h>
#include <tap.h>

int main(int argc, const char **argv) {
    tap_init(47);
    lex_context_t context;
    lex_token_t token;
    const char *line;
    char buffer[4096];
    if (lex_open(SRC_DIR "/test/hello-world/hello-world.zen", &context) < 0) {
        tap_fail(errno, "Failed to open file: %s", strerror(errno));
    }
#define LINE(expected) \
    line = expected;
#define TOKEN \
    if (lex_next_token(&context, &token) < 0) { \
        tap_fail(errno, "Failed to read token: %s", strerror(errno)); \
    } \
    if (lex_get_line(&token, buffer, 4096) < 0) { \
        tap_fail(errno, "Failed to read line: %s", strerror(errno)); \
    } \
    if (strcmp(line, buffer) == 0) { \
        tap_ok(NULL); \
    } else { \
        tap_not_ok("Expected '%s', but got '%s'.", line, buffer); \
    }
    LINE("#!/usr/bin/env zenith");
    LINE("");
    LINE("@@");
    TOKEN("@");
    TOKEN("@");
    LINE("");
    LINE("$msg << \"Hello, world!");
    TOKEN("$");
    TOKEN("msg");
    TOKEN("<");
    TOKEN("<");
    TOKEN("\"");
    TOKEN("Hello");
    TOKEN(",");
    TOKEN("world");
    TOKEN("!");
    LINE("\"");
    TOKEN("\"");
    LINE("$msg_end");
    TOKEN("$");
    TOKEN("msg_end");
    LINE("");
    LINE("@exec@");
    TOKEN("@");
    TOKEN("exec");
    TOKEN("@");
    LINE("");
    LINE("$write");
    TOKEN("$");
    TOKEN("write");
    LINE("    + 1");
    TOKEN("+");
    TOKEN("1");
    LINE("    + 4");
    TOKEN("+");
    TOKEN("4");
    LINE("    syscall");
    TOKEN("syscall");
    LINE("    --");
    TOKEN("-");
    TOKEN("-");
    LINE("    ret");
    TOKEN("ret");
    LINE("");
    LINE("$exit");
    TOKEN("$");
    TOKEN("exit");
    LINE("    + 1");
    TOKEN("+");
    TOKEN("1");
    LINE("    syscall");
    TOKEN("syscall");
    LINE("");
    LINE("$main");
    TOKEN("$");
    TOKEN("main");
    LINE("    + msg");
    TOKEN("+");
    TOKEN("msg");
    LINE("    + msg_end");
    TOKEN("+");
    TOKEN("msg_end");
    LINE("    sub");
    TOKEN("sub");
    LINE("    + msg");
    TOKEN("+");
    TOKEN("msg");
    LINE("    + write");
    TOKEN("+");
    TOKEN("write");
    LINE("    call");
    TOKEN("call");
    LINE("    + exit");
    TOKEN("+");
    TOKEN("exit");
    LINE("    call");
    TOKEN("call");
    if (lex_close(&context) < 0) {
        tap_fail(errno, "Failed to close file: %s", strerror(errno));
    }
    return 0;
}
