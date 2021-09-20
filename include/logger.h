#pragma once

#include <iostream>
#include <iomanip>

#if defined(__linux__) or defined(__APPLE__)
    #define RESET       "\033[0m"
    #define BLACK       "\033[30m"
    #define RED         "\033[31m"
    #define GREEN       "\033[32m"
    #define YELLOW      "\033[33m"
    #define BLUE        "\033[34m"
    #define MAGENTA     "\033[35m"
    #define CYAN        "\033[36m"
    #define WHITE       "\033[37m"
    #define BOLDBLACK   "\033[1m\033[30m"
    #define BOLDRED     "\033[1m\033[31m"
    #define BOLDGREEN   "\033[1m\033[32m"
    #define BOLDYELLOW  "\033[1m\033[33m"
    #define BOLDBLUE    "\033[1m\033[34m"
    #define BOLDMAGENTA "\033[1m\033[35m"
    #define BOLDCYAN    "\033[1m\033[36m"
    #define BOLDWHITE   "\033[1m\033[37m"
#else
    #define RESET       ""
    #define BLACK       ""
    #define RED         ""
    #define GREEN       ""
    #define YELLOW      ""
    #define BLUE        ""
    #define MAGENTA     ""
    #define CYAN        ""
    #define WHITE       ""
    #define BOLDBLACK   ""
    #define BOLDRED     ""
    #define BOLDGREEN   ""
    #define BOLDYELLOW  ""
    #define BOLDBLUE    ""
    #define BOLDMAGENTA ""
    #define BOLDCYAN    ""
    #define BOLDWHITE   ""
#endif

enum LogLevel
{
    Opcode,
    Trace,
    Debug,
    Warn,
    Error,
    Critical,
    None
};

class Logger : std::ostream
{
    public:
        Logger(LogLevel level = LogLevel::Error, std::ostream& stream = *s_DefaultStream)
            : m_LogLevel(level), m_Stream(stream), m_Open((m_LogLevel >= s_LogLevel) || ((m_LogLevel == LogLevel::Opcode) && s_OpcodeLogging))
        {
            if(m_Open)
            {
                switch(m_LogLevel)
                {
                    case LogLevel::Opcode:
                        stream << "[OPCODE] ";
                        break;
                    case LogLevel::Trace:
                        stream << "[" << GREEN << "TRACE" << RESET << "] ";
                        break;
                    case LogLevel::Debug:
                        stream << "[" << CYAN << "DEBUG" << RESET << "] ";
                        break;
                    case LogLevel::Warn:
                        stream << "[" << YELLOW << "WARN" << RESET << "] ";
                        break;
                    case LogLevel::Error:
                        stream << "[" << MAGENTA << "ERROR" << RESET << "] ";
                        break;
                    case LogLevel::Critical:
                        stream << BOLDRED << "[CRITICAL] ";
                    default:
                        break;
                }
            }
        }

        ~Logger()
        {
            if(m_Open)
            {
                m_Stream << RESET << "\n";
            }
        }

        template <class T>
        Logger& operator<<(const T& msg)
        {
            if(m_Open)
            {
                m_Stream << msg;
            }

            return *this;
        }

        Logger& operator<<(std::ostream& (*manip)(std::ostream&))
        {
            if(m_Open)
            {
                static_cast<std::ostream&>(*this) << manip;
            }

            return *this;
        }
        
        inline static void setLogLevel(LogLevel level) { s_LogLevel = level; }
        inline static void enableOpcodeLogging()  { s_OpcodeLogging = true; }
        inline static void disableOpcodeLogging() { s_OpcodeLogging = false; }
        inline static void setDefaultStream(std::ostream& stream) { s_DefaultStream = &stream; }
    private:
        LogLevel m_LogLevel;
        std::ostream& m_Stream;
        bool m_Open;

        static LogLevel s_LogLevel;
        static bool s_OpcodeLogging;
        static std::ostream* s_DefaultStream;
};

#define SET_LOG_LEVEL(x)        Logger::setLogLevel(x)
#define ENABLE_OP_LOGGING()     Logger::enableOpcodeLogging()
#define DISABLE_OP_LOGGING()    Logger::disableOpcodeLogging()

#define OPCODE(x)               Logger(LogLevel::Opcode) << x
#define TRACE(x)                Logger(LogLevel::Trace) << x
#define DEBUG(x)                Logger(LogLevel::Debug) << x
#define WARN(x)                 Logger(LogLevel::Warn) << x
#define ERROR(x)                Logger(LogLevel::Error, std::cerr) << x
#define CRITICAL(x)             Logger(LogLevel::Critical, std::cerr) << x