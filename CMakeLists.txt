cmake_minimum_required(VERSION 3.1) # Minimum required by NNG

project(mc_nng CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

option(NNG_TESTS "Build and run tests" OFF)
option(NNG_TOOLS "Build extra tools" OFF)
option(NNG_ENABLE_NNGCAT "Enable building nngcat utility." OFF)
add_definitions("-fPIC")
add_subdirectory(3rd-party/nng EXCLUDE_FROM_ALL)

add_subdirectory(src)
