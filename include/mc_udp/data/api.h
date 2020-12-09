/*
 * Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

// Handle portable symbol export.
// Defining manually which symbol should be exported is required
// under Windows whether MinGW or MSVC is used.
//
// The headers then have to be able to work in two different modes:
// - dllexport when one is building the library,
// - dllimport for clients using the library.
//
// On Linux, set the visibility accordingly. If C++ symbol visibility
// is handled by the compiler, see: http://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
// On Microsoft Windows, use dllimport and dllexport to tag symbols.
#  define MC_UDP_DATA_DLLIMPORT __declspec(dllimport)
#  define MC_UDP_DATA_DLLEXPORT __declspec(dllexport)
#  define MC_UDP_DATA_DLLLOCAL
#else
// On Linux, for GCC >= 4, tag symbols using GCC extension.
#  if __GNUC__ >= 4
#    define MC_UDP_DATA_DLLIMPORT __attribute__((visibility("default")))
#    define MC_UDP_DATA_DLLEXPORT __attribute__((visibility("default")))
#    define MC_UDP_DATA_DLLLOCAL __attribute__((visibility("hidden")))
#  else
// Otherwise (GCC < 4 or another compiler is used), export everything.
#    define MC_UDP_DATA_DLLIMPORT
#    define MC_UDP_DATA_DLLEXPORT
#    define MC_UDP_DATA_DLLLOCAL
#  endif // __GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

#ifdef MC_UDP_DATA_STATIC
// If one is using the library statically, get rid of
// extra information.
#  define MC_UDP_DATA_DLLAPI
#  define MC_UDP_DATA_LOCAL
#else
// Depending on whether one is building or using the
// library define DLLAPI to import or export.
#  ifdef MC_UDP_DATA_EXPORTS
#    define MC_UDP_DATA_DLLAPI MC_UDP_DATA_DLLEXPORT
#  else
#    define MC_UDP_DATA_DLLAPI MC_UDP_DATA_DLLIMPORT
#  endif // MC_UDP_DATA_EXPORTS
#  define MC_UDP_DATA_LOCAL MC_UDP_DATA_DLLLOCAL
#endif // MC_UDP_DATA_STATIC
