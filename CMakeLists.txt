cmake_minimum_required(VERSION 3.1) # Minimum required by NNG

project(mc_nng CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

add_subdirectory(src)
