/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include <iostream>
#include <sstream>

#ifndef WIN32

namespace mc_udp
{

static const char * OUT_NONE = "\033[00m";
static const char * OUT_BLUE = "\033[01;34m";
static const char * OUT_GREEN = "\033[01;32m";
static const char * OUT_PURPLE = "\033[01;35m";
static const char * OUT_RED = "\033[01;31m";

} // namespace mc_udp

#  define MC_UDP_ERROR(args) std::cerr << mc_udp::OUT_RED << args << mc_udp::OUT_NONE << std::endl;
#  define MC_UDP_WARNING(args) std::cerr << mc_udp::OUT_PURPLE << args << mc_udp::OUT_NONE << std::endl;
#  define MC_UDP_INFO(args) std::cout << mc_udp::OUT_BLUE << args << mc_udp::OUT_NONE << std::endl;
#  define MC_UDP_SUCCESS(args) std::cout << mc_udp::OUT_GREEN << args << mc_udp::OUT_NONE << std::endl;

#else

#  include <windows.h>
namespace mc_udp
{
static const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
static const int OUT_NONE = 15;
static const int OUT_BLUE = 11;
static const int OUT_GREEN = 10;
static const int OUT_PURPLE = 13;
static const int OUT_RED = 12;
} // namespace mc_udp

#  define MC_UDP_ERROR(args)                                    \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_RED); \
    std::cerr << args << std::endl;                             \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_NONE);

#  define MC_UDP_WARNING(args)                                     \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_PURPLE); \
    std::cerr << args << std::endl;                                \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_NONE);

#  define MC_UDP_INFO(args)                                      \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_BLUE); \
    std::cout << args << std::endl;                              \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_NONE);

#  define MC_UDP_SUCCESS(args)                                    \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_GREEN); \
    std::cout << args << std::endl;                               \
    SetConsoleTextAttribute(mc_udp::hConsole, mc_udp::OUT_NONE);

#endif

#define MC_UDP_ERROR_AND_THROW(exception_type, args) \
  {                                                  \
    std::stringstream strstrm;                       \
    strstrm << args;                                 \
    MC_UDP_ERROR(strstrm.str())                      \
    throw exception_type(strstrm.str());             \
  }
