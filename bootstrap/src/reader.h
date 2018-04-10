#ifndef ZENITH_READER_H
#define ZENITH_READER_H
#include <stddef.h>

#define SYMBOL_TYPE_COMMENT 0
#define SYMBOL_TYPE_VARIABLE 1
#define SYMBOL_TYPE_METHOD_START 2
#define SYMBOL_TYPE_METHOD_END 3
#define SYMBOL_TYPE_INSTRUCTION 4
#define SYMBOL_TYPE_EOF 5

#define VARIABLE_TYPE_CONSTANT 0
#define VARIABLE_TYPE_LENGTH 1
#define VARIABLE_TYPE_METHOD 2

#define INSTRUCTION_TYPE_PUSH 0
#define INSTRUCTION_TYPE_SYSCALL 1
#define INSTRUCTION_TYPE_IGNORE 2
#define INSTRUCTION_TYPE_CALL 3

typedef struct {
    int type;
    const char *name;
    const void *data;
    size_t data_length;
} symbol_variable_t;

typedef struct {
    const char *name;
} symbol_method_start_t;

typedef struct {
    int opcode;
    const char **arguments;
    size_t arguments_length;
} symbol_instruction_t;

typedef union {
    symbol_variable_t variable;
    symbol_method_start_t method_start;
    symbol_instruction_t instruction;
} symbol_data_t;

typedef struct {
    int type;
    symbol_data_t data;
} symbol_t;

int read_init(const char *filename);
int read_reset();
int read_symbol(symbol_t *symbol, void *data, size_t data_length);
int read_close();

#endif
