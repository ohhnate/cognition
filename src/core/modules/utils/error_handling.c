#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const char* log_level_strings[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};

void cog_log(CogLogLevel level, const char* file, int line, const char* fmt, ...) {
    va_list args;
    char time_buf[64];
    time_t raw_time;
    struct tm* time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(stderr, "[%s] %s (%s:%d): ", time_buf, log_level_strings[level], file, line);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    fflush(stderr);

    if (level == COG_LOG_FATAL) {
        exit(EXIT_FAILURE);
    }
}