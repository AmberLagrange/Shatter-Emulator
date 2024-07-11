#include <logging/logging.h>

#include <stdarg.h>
#include <stdio.h>

enum LogLevel MINIMUM_LOGGING_LEVEL = LOG_OPCODE;

#define ANSI_BLACK      "\x1b[0;30m"
#define ANSI_RED        "\x1b[0;31m"
#define ANSI_GREEN      "\x1b[0;32m"
#define ANSI_YELLOW     "\x1b[0;33m"
#define ANSI_BLUE       "\x1b[0;34m"
#define ANSI_MAGENTA    "\x1b[0;35m"
#define ANSI_CYAN       "\x1b[0;36m"
#define ANSI_WHITE      "\x1b[0;37m"
#define ANSI_RESET      "\x1b[0m"

void gameboy_log(enum LogLevel level, const char *fmt, ...) {

    if (level > MINIMUM_LOGGING_LEVEL) {
        return;
    }

    va_list args;
    va_start(args, fmt);

    switch(level) {

        case LOG_OPCODE:
            printf("[%sOPCODE%s]\t", ANSI_WHITE, ANSI_RESET);
            break;

        case LOG_TRACE:
            printf("[%sTRACE%s]\t", ANSI_GREEN, ANSI_RESET);
            break;

        case LOG_DEBUG:
            printf("[%sDEBUG%s]\t", ANSI_BLUE, ANSI_RESET);
            break;

        case LOG_INFO:
            printf("[%sINFO%s]\t", ANSI_GREEN, ANSI_RESET);
            break;

        case LOG_WARN:
            printf("[%sWARN%s]\t", ANSI_YELLOW, ANSI_RESET);
            break;

        case LOG_ERROR:
            printf("[%sERROR%s]\t", ANSI_RED, ANSI_RESET);
            break;

        case LOG_CRITICAL:
            printf("[%sCRITICAL%s]\t", ANSI_MAGENTA, ANSI_RESET);
            break;

        default:
            printf("[NONE]\t");
    }

    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}
