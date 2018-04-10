#include <errno.h>
#include <stdio.h>
#include <parser.h>
#include <stack.h>
#include <syscall.h>

int do_syscall(stack_t *stack) {
    operand_t operand;
    if (pop(stack, &operand) < 0) {
        return ~errno;
    }
    if (operand.is_constant) {
        switch (operand.value.constant) {
            case 1:
#include <syscall/independent/1.c>
            case 3:
#include <syscall/independent/3.c>
            case 4:
#include <syscall/independent/4.c>
            default:
                return ~(errno = ENOSYS);
        }
    } else {
        return ~(errno = EINVAL);
    }
}
