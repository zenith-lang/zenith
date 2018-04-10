#include <config.h>
#include <entry.h>
#include <streams.h>
#include <tap.h>

int main(int argc, const char **argv) {
    init_streams(&argc, argv);
    tap_init(2);
    const char *args[2];
    args[0] = "zenith-bootstrap";
    args[1] = SRC_DIR "/test/greeter/greeter.zen";
#define TEST_CASE(name) \
    set_stdin(name); \
    bootstrap_main(2, args); \
    if (is_stdin_empty() \
     && check_stream(stdout, "Hello!  What is your name?\n> \nNice to meet you, " name "!  My name is Zenith.\n") \
     && check_stream(stderr, "")) { \
        tap_ok(NULL); \
    } else { \
        tap_not_ok(NULL); \
    }
    TEST_CASE("Zach");
    TEST_CASE("Not Zach");
    return 0;
}
