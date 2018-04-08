#ifndef ZENITH_TEST_STREAMS_H
#define ZENITH_TEST_STREAMS_H
#include <stdio.h>

void init_streams(int *argc, const char **argv);
FILE *get_tap_log();
void set_stdin(const char *data);
int is_stdin_empty();
int check_stream(FILE *fd, const char *data);

#endif
