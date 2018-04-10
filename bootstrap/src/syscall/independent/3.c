{
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
    return fread(buf, 1, length, file);
}
