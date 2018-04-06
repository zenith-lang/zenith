#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <config.h>
#include <interpreter.h>
#include <main.h>
#include <parser.h>
#include <reader.h>

options_t options = {
    .run_type = RUN_TYPE_UNKNOWN
};

void print_version();
void print_usage(const char *argv0);

int main(int argc, const char **argv) {
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
        if (read_init(options.filename) < 0) {
            fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
            return errno;
        } else {
            program_t program;
            if (parse_program(&program) < 0) {
                fprintf(stderr, "Unable to parse program: %s\b", strerror(errno));
                if (read_close() < 0) {
                    fprintf(stderr, "Unable to close file: %s\n", strerror(errno));
                    return errno;
                }
            } else if (interpret_loop(&program) < 0) {
                fprintf(stderr, "Unable to interpret file: %s\n", strerror(errno));
                if (free_program(&program) < 0) {
                    fprintf(stderr, "Unable to free program: %s\n", strerror(errno));
                }
                if (read_close() < 0) {
                    fprintf(stderr, "Unable to close file: %s\n", strerror(errno));
                }
                return errno;
            } else if (read_close() < 0) {
                fprintf(stderr, "Unable to close file: %s\n", strerror(errno));
                if (free_program(&program) < 0) {
                    fprintf(stderr, "Unable to free program: %s\n", strerror(errno));
                }
                return errno;
            }
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
