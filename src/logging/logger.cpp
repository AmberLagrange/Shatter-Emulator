#include "logger.hpp"

#ifdef NDEBUG
    LogLevel Logger::s_LogLevel = LogLevel::Warn;
#else
    LogLevel Logger::s_LogLevel = LogLevel::Debug;
#endif

extern std::ostream std::cerr;

bool Logger::s_OpcodeLogging = false;
std::reference_wrapper<std::ostream> Logger::s_DefaultStream = std::cerr;

Logger::Logger(LogLevel level, std::ostream& stream)
            : m_LogLevel(level), m_Stream(stream),
              m_Open((m_LogLevel >= s_LogLevel) || ((m_LogLevel == LogLevel::Opcode) && s_OpcodeLogging))
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

Logger::~Logger()
{
    if(m_Open)
    {
        m_Stream << RESET << "\n";
    }
}

auto Logger::operator<<(std::ostream& (*manip)(std::ostream&)) -> Logger&
{
    if(m_Open)
    {
        static_cast<std::ostream&>(*this) << manip;
    }

    return *this;
}

void Logger::setLogLevel(LogLevel level)
{
    s_LogLevel = level;
}

void Logger::enableOpcodeLogging() 
{
    s_OpcodeLogging = true;
}

void Logger::disableOpcodeLogging()
{
    s_OpcodeLogging = false;
}

void Logger::setDefaultStream(std::ostream& stream)
{
    s_DefaultStream = stream;
}
