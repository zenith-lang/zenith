#ifndef ZENITH_TEST_TAP_H
#define ZENITH_TEST_TAP_H

void tap_init(int total);
void tap_ok(const char *comment, ...);
void tap_not_ok(const char *comment, ...);
void tap_fail(int exit_code, const char *comment, ...);

#endif
