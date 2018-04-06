#ifndef ZENITH_PARSER_H
#define ZENITH_PARSER_H

typedef struct {
    const char *name;
    void *data;
    int data_length;
} variable_t;

typedef union {
    variable_t *variable;
    int constant;
} operand_value_t;

typedef struct {
    int is_constant;
    operand_value_t value;
} operand_t;

typedef struct {
    int opcode;
    operand_t *operands;
    int operands_count;
} instruction_t;

typedef struct {
    const char *name;
    instruction_t *instructions;
    int instruction_count;
} method_t;

typedef struct {
    variable_t *variables;
    int variables_count;
    method_t *methods;
    int methods_count;
    method_t *main_method;
    void **allocs;
} program_t;

int parse_program(program_t *program);
int free_program(program_t *program);

#endif
