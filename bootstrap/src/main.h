#ifndef ZENITH_MAIN_H
#define ZENITH_MAIN_H

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

#endif
