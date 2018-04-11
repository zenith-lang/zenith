#ifndef ZENITH_ENTRY_H
#define ZENITH_ENTRY_H

#define RUN_TYPE_PARSE_FAILURE -2
#define RUN_TYPE_UNKNOWN -1
#define RUN_TYPE_INTERPRET 0
#define RUN_TYPE_HELP 1
#define RUN_TYPE_VERSION 2

typedef struct {
    int run_type;
    const char *filename;
} options_t;

extern options_t options;

int bootstrap_main(int argc, const char **argv);

#ifdef ZENITH_ENTRY_C

void print_version();
void print_usage(const char *argv0);
int do_interpret(const char *filename);

#endif
#endif
