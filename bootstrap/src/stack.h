#ifndef ZENITH_STACK_H
#define ZENITH_STACK_H

#define MAX_STACK_SIZE 4096

typedef struct {
    operand_t *values;
    int ptr;
    int size;
    int is_running;
} stack_t;

int push(stack_t *stack, operand_t *operand);
int pop(stack_t *stack, operand_t *operand);
int to_int(operand_t *operand, int *out);

#endif
