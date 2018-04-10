#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>
#include <reader.h>

#define INITIAL_BUFFER_LENGTH 4096
#define ALLOCS_LENGTH 6

int parse_program(program_t *program) {
    void *buffer = malloc(INITIAL_BUFFER_LENGTH);
    if (!buffer) {
        return ~(errno = ENOBUFS);
    }
    symbol_t symbol;
    program->variables_count = 0;
    program->methods_count = 0;
    program->main_method = NULL;
    int instruction_count = 0;
    int operand_count = 0;
    int length_count = 0;
    int is_reading = 1;
    int buffer_size = 0;
    while (is_reading) {
        int read = read_symbol(&symbol, buffer, buffer_size);
        if (read < 0) {
            free(buffer);
            return ~errno;
        } else {
            buffer_size += read;
        }
        switch (symbol.type) {
            case SYMBOL_TYPE_VARIABLE:
                ++program->variables_count;
                if (symbol.data.variable.type == VARIABLE_TYPE_LENGTH) {
                    ++length_count;
                }
                break;
            case SYMBOL_TYPE_METHOD_START:
                ++program->methods_count;
                ++program->variables_count;
                break;
            case SYMBOL_TYPE_INSTRUCTION:
                ++instruction_count;
                operand_count += symbol.data.instruction.arguments_length;
                break;
            case SYMBOL_TYPE_EOF:
                is_reading = 0;
                break;
        }
    }
    free(buffer);
    if (read_reset() < 0) {
        return ~errno;
    }
    program->allocs = (void **) malloc(sizeof(void *) * ALLOCS_LENGTH);
    if (!program->allocs) {
        return ~(errno = ENOBUFS);
    }
    program->allocs[0] = buffer = malloc(buffer_size);
    if (!buffer) {
        free(program->allocs);
        return ~(errno = ENOBUFS);
    }
    variable_t *variables = program->variables = (variable_t *) malloc(sizeof(variable_t) * program->variables_count);
    if (!variables) {
        free(program->allocs);
        free(buffer);
        return ~(errno = ENOBUFS);
    }
    program->allocs[1] = variables;
    method_t *methods = program->methods = (method_t *) malloc(sizeof(method_t) * program->methods_count);
    if (!methods) {
        free(program->allocs);
        free(buffer);
        free(program->methods);
        return ~(errno = ENOBUFS);
    }
    program->allocs[2] = methods;
    instruction_t *instructions = (instruction_t *) malloc(sizeof(instruction_t) * instruction_count);
    if (!instructions) {
        free(program->allocs);
        free(buffer);
        free(program->methods);
        free(instructions);
        return ~(errno = ENOBUFS);
    }
    program->allocs[3] = instructions;
    operand_t *operands = (operand_t *) malloc(sizeof(operand_t) * operand_count);
    if (!operands) {
        free(program->allocs);
        free(buffer);
        free(program->methods);
        free(instructions);
        free(operands);
        return ~(errno = ENOBUFS);
    }
    program->allocs[4] = operands;
    int *lengths = (int *) malloc(sizeof(int) * length_count);
    if (!lengths) {
        free(program->allocs);
        free(buffer);
        free(program->methods);
        free(instructions);
        free(operands);
        free(lengths);
        return ~(errno = ENOBUFS);
    }
    program->allocs[5] = lengths;
    is_reading = 1;
    while (is_reading) {
        int read = read_symbol(&symbol, buffer, buffer_size);
        if (read < 0) {
            return ~errno;
        }
        buffer += read;
        switch (symbol.type) {
            case SYMBOL_TYPE_VARIABLE:
                variables->name = symbol.data.variable.name;
                switch (symbol.data.variable.type) {
                    case VARIABLE_TYPE_CONSTANT:
                        variables->data = (void *) symbol.data.variable.data;
                        variables->data_length = symbol.data.variable.data_length;
                        break;
                    case VARIABLE_TYPE_LENGTH:
                        for (int i = 0; i < program->variables_count && variables != program->variables + i; ++i) {
                            if (strlen(program->variables[i].name) == symbol.data.variable.data_length && strncmp(symbol.data.variable.data, program->variables[i].name, symbol.data.variable.data_length) == 0) {
                                *lengths = program->variables[i].data_length;
                                variables->data = lengths++;
                            }
                        }
                        variables->data_length = sizeof(int);
                        break;
                    default:
                        return ~(errno = EINVAL);
                }
                ++variables;
                break;
            case SYMBOL_TYPE_METHOD_START:
                methods->name = symbol.data.method_start.name;
                methods->instructions = instructions;
                methods->instruction_count = 0;
                if (strcmp(methods->name, "main") == 0) {
                    program->main_method = methods;
                }
                variables->name = methods->name;
                variables->data = methods;
                variables->data_length = sizeof(method_t);
                ++variables;
                break;
            case SYMBOL_TYPE_METHOD_END:
                ++methods;
                break;
            case SYMBOL_TYPE_INSTRUCTION:
                instructions->opcode = symbol.data.instruction.opcode;
                instructions->operands = operands;
                instructions->operands_count = symbol.data.instruction.arguments_length;
                for (int i = 0; i < instructions->operands_count; ++i) {
                    operands->is_constant = 1;
                    for (int j = 0; symbol.data.instruction.arguments[i][j] != 0; ++j) {
                        if (symbol.data.instruction.arguments[i][j] < '0' || symbol.data.instruction.arguments[i][j] > '9') {
                            operands->is_constant = 0;
                            break;
                        }
                    }
                    if (operands->is_constant) {
                        sscanf(symbol.data.instruction.arguments[i], "%d", &operands->value.constant);
                    } else {
                        for (int j = 0; j < program->variables_count && variables != program->variables + j; ++j) {
                            if (strcmp(symbol.data.instruction.arguments[i], program->variables[j].name) == 0) {
                                operands->value.variable = program->variables + j;
                            }
                        }
                    }
                    ++operands;
                }
                ++methods->instruction_count;
                ++instructions;
                break;
            case SYMBOL_TYPE_EOF:
                is_reading = 0;
                break;
        }
    }
    return 0;
}

int free_program(program_t *program) {
    for (int i = 0; i < ALLOCS_LENGTH; ++i) {
        free(program->allocs[i]);
    }
    free(program->allocs);
}
