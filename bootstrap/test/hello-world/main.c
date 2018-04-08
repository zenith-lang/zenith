#include <config.h>
#include <entry.h>
#include <streams.h>
#include <tap.h>

int main(int argc, const char **argv) {
    init_streams(&argc, argv);
    tap_init(1);
    const char *args[2];
    args[0] = "zenith-bootstrap";
    args[1] = SRC_DIR "/test/hello-world/hello-world.zen";
    bootstrap_main(2, args);
    if (is_stdin_empty() && check_stream(stdout, "Hello, world!\n") && check_stream(stderr, "")) {
        tap_ok(NULL);
    } else {
        tap_not_ok(NULL);
    }
    return 0;
}
