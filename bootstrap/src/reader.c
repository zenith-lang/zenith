#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <reader.h>

FILE *read_file = NULL;
int is_in_method;

int read_init(const char *filename) {
    if (read_file != NULL) {
        return ~(errno = EALREADY);
    }
    read_file = fopen(filename, "r");
    is_in_method = 0;
    return read_file ? 0 : ~errno;
}

int read_reset() {
    fseek(read_file, 0, SEEK_SET);
    return 0;
}

int read_symbol(symbol_t *symbol, void *data, size_t data_length) {
    size_t start_length = data_length;
    long start_pos = ftell(read_file);
    char *it;
    while (1) {
        if (feof(read_file)) {
            symbol->type = SYMBOL_TYPE_EOF;
            return 0;
        }
        int c;
#define read_char() \
        c = fgetc(read_file); \
        if (c == EOF || ferror(read_file) != 0) { \
            fseek(read_file, start_pos, SEEK_SET); \
            return ~(errno = EIO); \
        }
#define WHITESPACE_CASES \
            case ' ': \
            case '\t': \
            case '\r': \
            case '\n':
#define WHITESPACE_IF(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        c = fgetc(read_file);
        if (c == EOF) {
            symbol->type = SYMBOL_TYPE_EOF;
            return 0;
        } else if (ferror(read_file) != 0) {
            fseek(read_file, start_pos, SEEK_SET);
            return ~(errno = EIO);
        }
        switch (c) {
            WHITESPACE_CASES
                break;
            case '#':
                symbol->type = SYMBOL_TYPE_COMMENT;
                do {
                    if (feof(read_file)) {
                        return 0;
                    }
                    read_char();
                } while (c != '\r' && c != '\n');
                return 0;
            case '$':
                symbol->type = SYMBOL_TYPE_METHOD_START;
                do {
                    read_char();
                } while (WHITESPACE_IF(c));
                it = data;
                symbol->data.method_start.name = (const char *) it;
                while (1) {
                    switch (c) {
                        WHITESPACE_CASES
                            break;
                        case '{':
                            if (--data_length < 0) {
                                fseek(read_file, start_pos, SEEK_SET);
                                return ~(errno = ENOBUFS);
                            }
                            *it = 0;
                            is_in_method = 1;
                            return start_length - data_length;
                        default:
                            if (--data_length < 0) {
                                fseek(read_file, start_pos, SEEK_SET);
                                return ~(errno = ENOBUFS);
                            }
                            *it++ = c;
                    }
                    if (feof(read_file)) {
                        fseek(read_file, start_pos, SEEK_SET);
                        return ~(errno = EIO);
                    }
                    read_char();
                }
            case '}':
                symbol->type = SYMBOL_TYPE_METHOD_END;
                is_in_method = 0;
                return 0;
            default:
                if (is_in_method) {
                    symbol->type = SYMBOL_TYPE_INSTRUCTION;
                    char *opcode = it = data;
                    int total_data_length = data_length;
                    while (1) {
                        if (WHITESPACE_IF(c) || c == ';') {
                            break;
                        } else if (--data_length < 0) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = ENOBUFS);
                        } else {
                            *it++ = c;
                        }
                        if (feof(read_file)) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = EIO);
                        }
                        read_char();
                    }
                    *it = 0;
                    int expected_args;
                    if (strcmp(opcode, "push") == 0) {
                        symbol->data.instruction.arguments_length = 1;
                        symbol->data.instruction.opcode = INSTRUCTION_TYPE_PUSH;
                    } else if (strcmp(opcode, "syscall") == 0) {
                        symbol->data.instruction.arguments_length = 0;
                        symbol->data.instruction.opcode = INSTRUCTION_TYPE_SYSCALL;
                    } else if (strcmp(opcode, "ignore") == 0) {
                        symbol->data.instruction.arguments_length = 0;
                        symbol->data.instruction.opcode = INSTRUCTION_TYPE_IGNORE;
                    } else {
                        fseek(read_file, start_pos, SEEK_SET);
                        return ~(errno = EIO);
                    }
                    data_length = total_data_length - sizeof(const char *) * symbol->data.instruction.arguments_length;
                    if (data_length < 0) {
                        fseek(read_file, start_pos, SEEK_SET);
                        return ~(errno = EIO);
                    }
                    symbol->data.instruction.arguments = data;
                    data += sizeof(const char *) * symbol->data.instruction.arguments_length;
                    for (int i = 0; i < symbol->data.instruction.arguments_length; ++i) {
                        it = (char *) data;
                        symbol->data.instruction.arguments[i] = (const char *) it;
                        while (1) {
                            if (feof(read_file)) {
                                fseek(read_file, start_pos, SEEK_SET);
                                return ~(errno = EIO);
                            }
                            read_char();
                            if (WHITESPACE_IF(c) || c == ';') {
                                break;
                            } else if (--data_length < 0) {
                                fseek(read_file, start_pos, SEEK_SET);
                                return ~(errno = ENOBUFS);
                            } else {
                                *it++ = c;
                                ++data;
                            }
                        }
                        if (--data_length < 0) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = ENOBUFS);
                        }
                        *it = 0;
                        ++data;
                    }
                    while (c != ';') {
                        if (feof(read_file)) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = EIO);
                        }
                        read_char();
                    }
                    return start_length - data_length;
                } else {
                    symbol->type = SYMBOL_TYPE_VARIABLE;
                    it = data;
                    symbol->data.variable.name = (const char *) it;
                    while (1) {
                        if (WHITESPACE_IF(c)) {
                            break;
                        } else if (--data_length < 0) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = ENOBUFS);
                        } else {
                            *it++ = c;
                        }
                        if (feof(read_file)) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = EIO);
                        }
                        read_char();
                    }
                    if (--data_length < 0) {
                        fseek(read_file, start_pos, SEEK_SET);
                        return ~(errno = ENOBUFS);
                    }
                    *it++ = 0;
                    do {
                        if (feof(read_file)) {
                            fseek(read_file, start_pos, SEEK_SET);
                            return ~(errno = EIO);
                        }
                        read_char();
                    } while (WHITESPACE_IF(c));
                    switch (c) {
                        case '"':
                            symbol->data.variable.type = VARIABLE_TYPE_CONSTANT;
                            int is_escaped = 0;
                            symbol->data.variable.data = it;
                            symbol->data.variable.data_length = 0;
                            while (1) {
                                if (feof(read_file)) {
                                    fseek(read_file, start_pos, SEEK_SET);
                                    return ~(errno = EIO);
                                }
                                read_char();
                                switch (c) {
                                    case '\\':
                                        if (is_escaped) {
                                            if (--data_length < 0) {
                                                fseek(read_file, start_pos, SEEK_SET);
                                                return ~(errno = ENOBUFS);
                                            }
                                            *it++ = '\\';
                                            ++symbol->data.variable.data_length;
                                            is_escaped = 0;
                                        } else {
                                            is_escaped = 1;
                                        }
                                        break;
                                    case '"':
                                        if (is_escaped) {
                                            if (--data_length < 0) {
                                                fseek(read_file, start_pos, SEEK_SET);
                                                return ~(errno = ENOBUFS);
                                            }
                                            *it++ = '"';
                                            ++symbol->data.variable.data_length;
                                            is_escaped = 0;
                                        } else {
                                            return start_length - data_length;
                                        }
                                        break;
                                    default:
                                        if (is_escaped) {
                                            if (--data_length < 0) {
                                                fseek(read_file, start_pos, SEEK_SET);
                                                return ~(errno = ENOBUFS);
                                            }
                                            *it++ = '\\';
                                            ++symbol->data.variable.data_length;
                                            is_escaped = 0;
                                        }
                                        if (--data_length < 0) {
                                            fseek(read_file, start_pos, SEEK_SET);
                                            return ~(errno = ENOBUFS);
                                        }
                                        *it++ = c;
                                        ++symbol->data.variable.data_length;
                                        break;
                                }
                            }
                            break;
                        case '#':
                            symbol->data.variable.type = VARIABLE_TYPE_LENGTH;
                            symbol->data.variable.data = it;
                            symbol->data.variable.data_length = 0;
                            while (1) {
                                if (feof(read_file)) {
                                    fseek(read_file, start_pos, SEEK_SET);
                                    return ~(errno = EIO);
                                }
                                read_char();
                                if (c == ';') {
                                    return start_length - data_length;
                                } else if (--data_length < 0) {
                                    fseek(read_file, start_pos, SEEK_SET);
                                    return ~(errno = ENOBUFS);
                                } else {
                                    *it++ = c;
                                    ++symbol->data.variable.data_length;
                                }
                            }
                    }
                }
        }
#undef WHITESPACE_IF
#undef WHITESPACE_CASES
#undef read_char
    }
    return ~(errno = ENOSYS);
}

int read_close() {
    if (fclose(read_file) == 0) {
        read_file = NULL;
        return 0;
    } else {
        return ~(errno = EIO);
    }
}
