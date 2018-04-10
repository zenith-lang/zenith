#include <config.h>
#include <entry.h>
#include <streams.h>
#include <tap.h>

int main(int argc, const char **argv) {
    init_streams(&argc, argv);
    tap_init(1);
    const char *args[2];
    args[0] = "zenith-bootstrap";
    args[1] = SRC_DIR "/test/arithmetic/arithmetic.zen";
    bootstrap_main(2, args);
    if (is_stdin_empty() && check_stream(stdout,
            "3 + 2 = 5\n"
            "3 - 2 = 1\n"
            "3 x 2 = 6\n"
            "3 / 2 = 1\n"
        ) && check_stream(stderr, "")) {
        tap_ok(NULL);
    } else {
        tap_not_ok(NULL);
    }
    return 0;
}
