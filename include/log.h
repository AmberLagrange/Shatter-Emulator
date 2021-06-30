#pragma once

#ifndef NDEBUG
    #include <iostream>
    #include <iomanip>

    #define LOG(x) std::cout << x << "\n"
    #define ERROR(x) std::cerr << x << std::endl
#else
    #define LOG(x) ((void)0)
    #define ERROR(x) ((void)0)
#endif