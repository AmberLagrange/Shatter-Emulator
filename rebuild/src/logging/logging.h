#ifndef LOGGING_H
#define LOGGING_H

enum LogLevel
{
    LOG_CRITICAL    = 0,
    LOG_ERROR       = 1,
    LOG_WARN        = 2,
    LOG_INFO        = 3,
    LOG_DEBUG       = 4,
    LOG_TRACE       = 5,
    LOG_OPCODE      = 6,
};

extern enum LogLevel MINIMUM_LOGGING_LEVEL;

/**
 * @brief Logs information to stdout by prepending the log level to the provided message
 * 
 * @todo Set a base log level and ignore any logs below said level
 *
 * @param level 
 * @param fmt 
 * @param ... 
 */
void gameboy_log(enum LogLevel level, const char *fmt, ...);

#endif // LOGGING_H
