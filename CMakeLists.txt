cmake_minimum_required(VERSION 3.1) # Minimum required by recent mc_rtc

project(mc_udp CXX)
set(CMAKE_CXX_STANDARD 11)

option(BUILD_MC_RTC_CLIENT "Build with mc_rtc client" ON)
option(BUILD_DUMMY_CLIENT "Build with dummy client" ON)
option(BUILD_OPENRTM_SERVER "Build with openrtm server" ON)

find_package(PkgConfig REQUIRED)

add_subdirectory(src)

install(
  FILES "${CMAKE_SOURCE_DIR}/mc_udpConfig.cmake"
  DESTINATION lib/cmake/mc_udp
)
