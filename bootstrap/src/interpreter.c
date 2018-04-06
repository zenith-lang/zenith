#include <errno.h>
#include <stdlib.h>
#include <interpreter.h>
#include <parser.h>
#include <reader.h>
#include <stack.h>
#include <syscall.h>

int execute_method(program_t *program, stack_t *stack, method_t *method);
int execute_instruction(program_t *program, stack_t *stack, instruction_t *instruction);

int interpret_loop(program_t *program) {
    if (!program->main_method) {
        return ~(errno = ELIBEXEC);
    }
    stack_t stack = {
        .ptr = 0,
        .size = MAX_STACK_SIZE,
        .is_running = 1
    };
    stack.values = (operand_t *) malloc(sizeof(stack_t) * MAX_STACK_SIZE);
    return execute_method(program, &stack, program->main_method);
}

int execute_method(program_t *program, stack_t *stack, method_t *method) {
    for (int i = 0; i < method->instruction_count && stack->is_running; ++i) {
        if (execute_instruction(program, stack, method->instructions + i) < 0) {
            return ~errno;
        }
    }
    return 0;
}

int execute_instruction(program_t *program, stack_t *stack, instruction_t *instruction) {
    switch (instruction->opcode) {
        case INSTRUCTION_TYPE_PUSH:
            if (push(stack, instruction->operands) < 0) {
                return ~errno;
            }
            break;
        case INSTRUCTION_TYPE_SYSCALL:
            if (do_syscall(stack) < 0) {
                return ~errno;
            }
            break;
        default:
            return ~(errno = EINVAL);
    }
    return 0;
}
