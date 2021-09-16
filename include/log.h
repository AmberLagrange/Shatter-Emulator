#pragma once

#ifndef NDEBUG
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

    #define LOG(x) std::cout << x << "\n" 
    #define WARN(x) std::cout << YELLOW << x << RESET << std::endl 
    #define ERROR(x) std::cerr << RED << x << RESET << std::endl
#else
    #define LOG(x) ((void)0)
    #define ERROR(x) ((void)0)
#endif