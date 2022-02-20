#include "logger.h"

#ifdef NDEBUG
    LogLevel Logger::s_LogLevel = LogLevel::Warn;
#else
    LogLevel Logger::s_LogLevel = LogLevel::Debug;
#endif

bool Logger::s_OpcodeLogging = false;
std::ostream* Logger::s_DefaultStream = &std::cout;