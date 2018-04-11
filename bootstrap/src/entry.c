#define ZENITH_ENTRY_C
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <config.h>
#include <entry.h>

options_t options;

int bootstrap_main(int argc, const char **argv) {
    options.run_type = RUN_TYPE_UNKNOWN;
    for (const char **it = argv + 1, **end = argv + argc; it != end; ++it) {
        const char *arg = *it;
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            if (options.run_type == RUN_TYPE_UNKNOWN) {
                options.run_type = RUN_TYPE_HELP;
            } else {
                fprintf(stderr, "Extra option '%s'\n", arg);
                options.run_type = RUN_TYPE_PARSE_FAILURE;
            }
        } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            if (options.run_type == RUN_TYPE_UNKNOWN) {
                options.run_type = RUN_TYPE_VERSION;
            } else {
                fprintf(stderr, "Extra option '%s'\n", arg);
                options.run_type = RUN_TYPE_PARSE_FAILURE;
            }
        } else {
            if (options.run_type == RUN_TYPE_UNKNOWN) {
                options.run_type = RUN_TYPE_INTERPRET;
                options.filename = arg;
            } else {
                fprintf(stderr, "Unknown option '%s'\n", arg);
                options.run_type = RUN_TYPE_PARSE_FAILURE;
            }
        }
    }
    if (options.run_type < 0 || options.run_type == RUN_TYPE_HELP) {
        print_usage(*argv);
    } else if (options.run_type == RUN_TYPE_VERSION) {
        print_version();
    } else {
        int err = do_interpret(options.filename);
        if (err < 0) {
            fprintf(stderr, "Unable to interpret file: %s\n", strerror(errno));
            return err;
        }
    }
    return options.run_type < 0 ? 1 : 0;
}

void print_version() {
    FILE *stream = options.run_type < 0 ? stderr : stdout;
    fputs(PACKAGE_STRING "\n", stream);
}

void print_usage(const char *argv0) {
    FILE *stream = options.run_type < 0 ? stderr : stdout;
    fprintf(stream, "Usage: %s [options] [filename]\n", argv0);
    fputs("\n", stream);
    fputs("Options:\n", stream);
    fputs("    -h, --help     Display this help message and exit\n", stream);
    fputs("    -v, --version  Display the version information and exit\n", stream);
    fputs("\n", stream);
    print_version();
}

int do_interpret(const char *filename) {
    return ~(errno = ENOSYS);
}
