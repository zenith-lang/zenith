#ifndef ZENITH_LEX_H
#define ZENITH_LEX_H
#include <stdio.h>

#define MAX_TOKEN_LENGTH 64

typedef struct {
    FILE *file;
    int line_start;
    int line_end;
    int token_start;
    char *line;
} lex_context_t;

typedef struct {
    lex_context_t *context;
    int line_start;
    int line_end;
    int token_start;
    int token_end;
    char value[MAX_TOKEN_LENGTH];
} lex_token_t;

int lex_open(const char *filename, lex_context_t *context);
int lex_next_token(lex_context_t *context, lex_token_t *token);
int lex_get_line(lex_token_t *token, char *line, int size);
int lex_read_between(lex_token_t *start, lex_token_t *end, char *buf, int size);
int lex_close(lex_context_t *context);

#ifdef ZENITH_LEX_C

int lex_read_line(lex_context_t *context);

#endif
#endif
