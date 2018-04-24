#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <streams.h>

#define TAP_LOG_ARG "--tap-log="
#define BUFFER_SIZE 4096

FILE *tap_log = NULL;
char tap_log_name[L_tmpnam];
char stdin_name[L_tmpnam];
char stdout_name[L_tmpnam];
char stderr_name[L_tmpnam];
int stdin_len;

void cleanup_streams();
void cleanup_streams_sig(int sig);

void init_streams(int *argc, const char **argv) {
    atexit(&cleanup_streams);
    signal(SIGABRT, &cleanup_streams);
    signal(SIGFPE, &cleanup_streams);
    signal(SIGILL, &cleanup_streams);
    signal(SIGINT, &cleanup_streams);
    signal(SIGSEGV, &cleanup_streams);
    signal(SIGTERM, &cleanup_streams);
    if (*argc > 1 && strncmp(argv[*argc - 1], TAP_LOG_ARG, strlen(TAP_LOG_ARG)) == 0) {
        tap_log = fopen(argv[*argc - 1] + strlen(TAP_LOG_ARG), "a");
        --*argc;
        (void) (tmpnam(stdin_name) + 1);
        (void) (tmpnam(stdout_name) + 1);
        (void) (tmpnam(stderr_name) + 1);
        (void) (freopen(stdin_name, "w+", stdin) + 1);
        (void) (freopen(stdout_name, "w+", stdout) + 1);
        (void) (freopen(stderr_name, "w+", stderr) + 1);
    } else if (system(NULL)) {
        (void) (tmpnam(tap_log_name) + 1);
        int command_length = 3 + strlen(TAP_LOG_ARG) + strlen(tap_log_name);
        for (int i = 0; i < *argc; ++i) {
            command_length += 3;
            for (int j = 0; argv[i][j]; ++j) {
                if (argv[i][j] == '"') {
                    command_length += 2;
                } else {
                    ++command_length;
                }
            }
        }
        char *command = (char *) malloc(command_length);
        char *it = command;
        for (int i = 0; i < *argc; ++i) {
            *it++ = '"';
            for (int j = 0; argv[i][j]; ++j) {
                if (argv[i][j] == '"') {
                    *it++ = '\\';
                    *it++ = '"';
                } else {
                    *it++ = argv[i][j];
                }
            }
            *it++ = '"';
            *it++ = ' ';
        }
        strcpy(it, "\"" TAP_LOG_ARG);
        it += 1 + strlen(TAP_LOG_ARG);
        strcpy(it, tap_log_name);
        it += strlen(tap_log_name);
        strcpy(it, "\"");
        int exit_code = system(command);
        tap_log = fopen(tap_log_name, "r");
        if (tap_log) {
            char buffer[BUFFER_SIZE];
            size_t read;
            while ((read = fread(buffer, 1, BUFFER_SIZE, tap_log)) > 0) {
                fwrite(buffer, 1, read, stdout);
            }
            exit(exit_code);
        } else {
            fputs("Unable to open TAP log.\n", stderr);
            exit(1);
        }
    } else {
        fputs("Unable to spawn new process.\n", stderr);
        exit(1);
    }
}

void cleanup_streams() {
    if (tap_log) {
        fclose(tap_log);
    }
    FILE *tmp = fopen(tap_log_name, "r");
    if (tmp) {
        fclose(tmp);
        remove(tap_log_name);
    }
    fclose(stdin);
    tmp = fopen(stdin_name, "r");
    if (tmp) {
        fclose(tmp);
        remove(stdin_name);
    }
    fclose(stdout);
    tmp = fopen(stdout_name, "r");
    if (tmp) {
        fclose(tmp);
        remove(stdout_name);
    }
    fclose(stderr);
    tmp = fopen(stderr_name, "r");
    if (tmp) {
        fclose(tmp);
        remove(stderr_name);
    }
    exit(0);
}

void cleanup_streams_sig(int sig) {
    cleanup_streams();
}

FILE *get_tap_log() {
    if (tap_log) {
        return tap_log;
    } else {
        return stdout;
    }
}

void set_stdin(const char *data) {
    fseek(stdin, 0, SEEK_SET);
    fputs(data, stdin);
    fseek(stdin, 0, SEEK_SET);
    stdin_len = strlen(data);
}

int is_stdin_empty() {
    return ftell(stdin) == stdin_len;
}

int check_stream(FILE *fd, const char *data) {
    int pos = ftell(fd);
    if (pos != strlen(data)) {
        return 0;
    }
    fseek(fd, 0, SEEK_SET);
    while (*data) {
        if (*data++ != fgetc(fd)) {
            fseek(fd, 0, SEEK_SET);
            return 0;
        }
    }
    fseek(fd, 0, SEEK_SET);
    return 1;
}
