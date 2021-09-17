#include "logger.h"

LogLevel Logger::s_LogLevel = LogLevel::Debug;

void Logger::setLogLevel(LogLevel level) { s_LogLevel = level; }