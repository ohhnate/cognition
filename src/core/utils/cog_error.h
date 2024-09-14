#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdarg.h>

typedef enum {
    COG_LOG_DEBUG,
    COG_LOG_INFO,
    COG_LOG_WARNING,
    COG_LOG_ERROR,
    COG_LOG_FATAL
} CogLogLevel;

void cog_log(CogLogLevel level, const char* file, int line, const char* fmt, ...);

#define cog_log_debug(...) cog_log(COG_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define cog_log_info(...) cog_log(COG_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define cog_log_warning(...) cog_log(COG_LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define cog_log_error(...) cog_log(COG_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define cog_log_fatal(...) cog_log(COG_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif // ERROR_HANDLING_H