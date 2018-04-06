#include <errno.h>
#include <parser.h>
#include <stack.h>

int push(stack_t *stack, operand_t *operand) {
    if (stack->ptr + 1 >= stack->size) {
        return ~(errno = ENOBUFS);
    }
    stack->values[stack->ptr++] = *operand;
    return 0;
}

int pop(stack_t *stack, operand_t *operand) {
    if (stack->ptr <= 0) {
        return ~(errno = ENOBUFS);
    }
    *operand = stack->values[--stack->ptr];
    return 0;
}

int to_int(operand_t *operand, int *out) {
    if (operand->is_constant) {
        *out = operand->value.constant;
    } else {
        *out = *(int *) operand->value.variable->data;
    }
    return 0;
}
