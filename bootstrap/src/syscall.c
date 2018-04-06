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
                stack->is_running = 0;
                break;
            case 4:
                if (pop(stack, &operand) < 0) {
                    return ~errno;
                }
                int fd;
                if (to_int(&operand, &fd) < 0) {
                    return ~errno;
                }
                FILE *file;
                switch (fd) {
                    case 0:
                        file = stdin;
                        break;
                    case 1:
                        file = stdout;
                        break;
                    case 2:
                        file = stderr;
                        break;
                    default:
                        return ~(errno = ENOSYS);
                }
                if (pop(stack, &operand) < 0) {
                    return ~errno;
                }
                if (operand.is_constant) {
                    return ~(errno = EINVAL);
                }
                void *buf = operand.value.variable->data;
                if (pop(stack, &operand) < 0) {
                    return ~errno;
                }
                int length;
                if (to_int(&operand, &length) < 0) {
                    return ~errno;
                }
                fwrite(buf, 1, length, file);
                break;
            default:
                return ~(errno = ENOSYS);
        }
    } else {
        return ~(errno = EINVAL);
    }
}
