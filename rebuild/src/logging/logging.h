#ifndef LOGGING_H
#define LOGGING_H

enum LogLevel
{
    LOG_OPCODE,
    LOG_TRACE,
    LOG_DEBUG,
    LOG_WARN,
    LOG_ERROR,
    LOG_CRITICAL,
};

void gameboy_log(enum LogLevel level, const char *fmt, ...);

#endif // LOGGING_H
