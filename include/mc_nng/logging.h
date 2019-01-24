#pragma once

#include <iostream>
#include <sstream>

#ifndef WIN32

namespace mc_nng
{

constexpr auto OUT_NONE = "\033[00m";
constexpr auto OUT_BLUE = "\033[01;34m";
constexpr auto OUT_GREEN = "\033[01;32m";
constexpr auto OUT_PURPLE = "\033[01;35m";
constexpr auto OUT_RED = "\033[01;31m";

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
constexpr auto OUT_NONE = 15;
constexpr auto OUT_BLUE = 11;
constexpr auto OUT_GREEN = 10;
constexpr auto OUT_PURPLE = 13;
constexpr auto OUT_RED = 12;
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
