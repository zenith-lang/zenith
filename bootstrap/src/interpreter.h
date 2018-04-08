#ifndef ZENITH_INTERPRETER_H
#define ZENITH_INTERPRETER_H
#include <parser.h>
#include <stack.h>

int interpret_loop(program_t *program);

#ifdef ZENITH_INTERPRETER_C

int execute_method(program_t *program, stack_t *stack, method_t *method);
int execute_instruction(program_t *program, stack_t *stack, instruction_t *instruction);

#endif
#endif
