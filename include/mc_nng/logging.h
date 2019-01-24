#pragma once

#include <iostream>
#include <sstream>

#ifndef WIN32

namespace mc_nng
{

static const char * OUT_NONE = "\033[00m";
static const char * OUT_BLUE = "\033[01;34m";
static const char * OUT_GREEN = "\033[01;32m";
static const char * OUT_PURPLE = "\033[01;35m";
static const char * OUT_RED = "\033[01;31m";

} // namespace mc_nng

#  define MC_NNG_ERROR(args) std::cerr << mc_nng::OUT_RED << args << mc_nng::OUT_NONE << std::endl;
#  define MC_NNG_WARNING(args) std::cerr << mc_nng::OUT_PURPLE << args << mc_nng::OUT_NONE << std::endl;
#  define MC_NNG_INFO(args) std::cout << mc_nng::OUT_BLUE << args << mc_nng::OUT_NONE << std::endl;
#  define MC_NNG_SUCCESS(args) std::cout << mc_nng::OUT_GREEN << args << mc_nng::OUT_NONE << std::endl;

#else

#  include <windows.h>
namespace mc_nng
{
static const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
static const int OUT_NONE = 15;
static const int OUT_BLUE = 11;
static const int OUT_GREEN = 10;
static const int OUT_PURPLE = 13;
static const int OUT_RED = 12;
} // namespace mc_nng

#  define MC_NNG_ERROR(args)                                       \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_RED); \
    std::cerr << args << std::endl;                             \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_NONE);

#  define MC_NNG_WARNING(args)                                        \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_PURPLE); \
    std::cerr << args << std::endl;                                \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_NONE);

#  define MC_NNG_INFO(args)                                         \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_BLUE); \
    std::cout << args << std::endl;                              \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_NONE);

#  define MC_NNG_SUCCESS(args)                                       \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_GREEN); \
    std::cout << args << std::endl;                               \
    SetConsoleTextAttribute(mc_nng::hConsole, mc_nng::OUT_NONE);

#endif

#define MC_NNG_ERROR_AND_THROW(exception_type, args) \
  {                                               \
    std::stringstream strstrm;                    \
    strstrm << args;                              \
    MC_NNG_ERROR(strstrm.str())                      \
    throw exception_type(strstrm.str());          \
  }
