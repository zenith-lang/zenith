#define ZENITH_LEX_C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <lex.h>

int lex_open(const char *filename, lex_context_t *context) {
    context->file = fopen(filename, "r");
    context->line = NULL;
    return lex_read_line(context);
}

int lex_next_token(lex_context_t *context, lex_token_t *token) {
    int c;
    token->context = context;
    do {
        while (context->line[context->token_start] == ' '  ||
            context->line[context->token_start] == '\t' ||
            context->line[context->token_start] == '\n' ||
            context->line[context->token_start] == '\r' ||
            context->line[context->token_start] == '\0') {
            if (++context->token_start >= context->line_end - context->line_start) {
                if (lex_read_line(context) < 0) {
                    return ~errno;
                }
            }
        }
        token->line_start = context->line_start;
        token->line_end = context->line_end;
        token->token_end = token->token_start = context->token_start;
        int c = context->line[token->token_end++];
        char *it = token->value;
        if (c == '#') {
            if (lex_read_line(context) < 0) {
                return ~errno;
            }
            continue;
        } else if (c >= '0' && c <= '9') {
            do {
                *it++ = c;
                c = context->line[token->token_end++];
            } while (c >= '0' && c <= '9');
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            do {
                *it++ = c;
                c = context->line[token->token_end++];
            } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_');
        } else if (c >= '!' && c <= '~') {
            *it++ = c;
            token->token_end++;
        } else {
            return ~(errno = EIO);
        }
        *it = 0;
        context->token_start = token->token_end - 1;
    } while (*token->value == 0);
    return 0;
}

int lex_get_line(lex_token_t *token, char *line, size_t size) {
    int len = token->line_end - token->line_start;
    if (size < len) {
        return ~(errno = ENOBUFS);
    }
    int pos = ftell(token->context->file);
    fseek(token->context->file, token->line_start, SEEK_SET);
    if (fread(line, 1, len, token->context->file) < 0) {
        fseek(token->context->file, pos, SEEK_SET);
        return ~(errno = EIO);
    }
    line[len] = 0;
    fseek(token->context->file, pos, SEEK_SET);
    return 0;
}

int lex_read_between(lex_token_t *start, lex_token_t *end, char *buf, size_t size) {
    int len = end->token_start + end->line_start - start->token_end - start->line_start + 1;
    if (len < 0 || start->context != end->context) {
        return ~(errno = EINVAL);
    }
    if (size < len) {
        return ~(errno = ENOBUFS);
    }
    int pos = ftell(start->context->file);
    fseek(start->context->file, start->token_end + start->line_start - 1, SEEK_SET);
    if (fread(buf, 1, len, start->context->file) < 0) {
        fseek(start->context->file, pos, SEEK_SET);
        return ~(errno = EIO);
    }
    buf[len] = 0;
    fseek(start->context->file, pos, SEEK_SET);
    return 0;
}

int lex_close(lex_context_t *context) {
    fclose(context->file);
    context->file = NULL;
    if (context->line) {
        free(context->line);
    }
    return 0;
}

int lex_read_line(lex_context_t *context) {
    context->line_end = context->line_start = ftell(context->file);
    int len = 0;
    for (int c = fgetc(context->file); c != EOF && c != '\n'; c = fgetc(context->file)) {
        ++len;
    }
    context->line_end = context->line_start + len;
    fseek(context->file, context->line_start, SEEK_SET);
    if (context->line) {
        free(context->line);
    }
    context->line = (char *) malloc(len);
    if (!context->line) {
        return ~(errno = ENOMEM);
    }
    if (fread(context->line, 1, len, context->file) < 0) {
        return ~(errno = EIO);
    }
    context->line[len] = 0;
    fseek(context->file, 1, SEEK_CUR);
    context->token_start = 0;
    return 0;
}
