#pragma once

#include <functional>
#include <iomanip>
#include <iostream>

#if defined(__linux__) or defined(__APPLE__)
    constexpr auto RESET       = "\033[0m";
    constexpr auto BLACK       = "\033[30m";
    constexpr auto RED         = "\033[31m";
    constexpr auto GREEN       = "\033[32m";
    constexpr auto YELLOW      = "\033[33m";
    constexpr auto BLUE        = "\033[34m";
    constexpr auto MAGENTA     = "\033[35m";
    constexpr auto CYAN        = "\033[36m";
    constexpr auto WHITE       = "\033[37m";
    constexpr auto BOLDBLACK   = "\033[1m\033[30m";
    constexpr auto BOLDRED     = "\033[1m\033[31m";
    constexpr auto BOLDGREEN   = "\033[1m\033[32m";
    constexpr auto BOLDYELLOW  = "\033[1m\033[33m";
    constexpr auto BOLDBLUE    = "\033[1m\033[34m";
    constexpr auto BOLDMAGENTA = "\033[1m\033[35m";
    constexpr auto BOLDCYAN    = "\033[1m\033[36m";
    constexpr auto BOLDWHITE   = "\033[1m\033[37m";
#else
    constexpr auto RESET       = "";
    constexpr auto BLACK       = "";
    constexpr auto RED         = "";
    constexpr auto GREEN       = "";
    constexpr auto YELLOW      = "";
    constexpr auto BLUE        = "";
    constexpr auto MAGENTA     = "";
    constexpr auto CYAN        = "";
    constexpr auto WHITE       = "";
    constexpr auto BOLDBLACK   = "";
    constexpr auto BOLDRED     = "";
    constexpr auto BOLDGREEN   = "";
    constexpr auto BOLDYELLOW  = "";
    constexpr auto BOLDBLUE    = "";
    constexpr auto BOLDMAGENTA = "";
    constexpr auto BOLDCYAN    = "";
    constexpr auto BOLDWHITE   = "";
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

template <class T>
concept Printable = requires(std::ostream& stream, T t)
{
    stream << t;
};

class Logger final : std::ostream
{
    public:
        Logger(LogLevel level = LogLevel::Error, std::ostream& stream = s_DefaultStream);
        virtual ~Logger() final;

        /**
         * @brief Generic stream insertion operator overload for any
         * message that can be inserted into a stream
         * 
         * @tparam T Printable class (i.e. stream << T)
         * @param msg The message to print
         * @return Logger& The updated logger
         */
        template <Printable T> auto operator<<(const T& msg) -> Logger&;


        /**
         * @brief Overloaded stream insertion operator for io manipulation
         * 
         * @param manip The io manipulation function pointer
         * @return Logger& The updated logger
         */
        auto operator<<(std::ostream& (*manip)(std::ostream&)) -> Logger&;
        
        /**
         * @brief Set the logging level
         * 
         * @param level The level to start logging at
         */
        static void setLogLevel(LogLevel level);

        /**
         * @brief Enable opcode logging
         * 
         */
        static void enableOpcodeLogging();

        /**
         * @brief Disable opcode logging
         * 
         */
        static void disableOpcodeLogging();

        /**
         * @brief Set the Default Stream
         * 
         * @param stream The stream to set as the default
         */
        static void setDefaultStream(std::ostream& stream);
    private:
        LogLevel m_LogLevel;
        std::ostream& m_Stream;
        bool m_Open;

        static LogLevel s_LogLevel;
        static bool s_OpcodeLogging;
        static std::reference_wrapper<std::ostream> s_DefaultStream;
};

template <Printable T>
auto Logger::operator<<(const T& msg) -> Logger&
{
    if(m_Open)
    {
        m_Stream << msg;
    }

    return *this;
}

#define OPCODE(x)               Logger(LogLevel::Opcode) << x
#define TRACE(x)                Logger(LogLevel::Trace) << x
#define DEBUG(x)                Logger(LogLevel::Debug) << x
#define WARN(x)                 Logger(LogLevel::Warn) << x
#define ERROR(x)                Logger(LogLevel::Error, std::cerr) << x
#define CRITICAL(x)             Logger(LogLevel::Critical, std::cerr) << x
