#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

set(client_HDR
  "${include_DIR}/client/api.h"
  "${include_DIR}/client/Client.h"
)
set(client_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/Client.cpp"
)

add_library(mc_udp_client SHARED ${client_SRC} ${client_HDR})
add_library(mc_udp::client ALIAS mc_udp_client)
set_target_properties(mc_udp_client PROPERTIES EXPORT_NAME client)
set_target_properties(mc_udp_client PROPERTIES COMPILE_FLAGS "-DMC_UDP_CLIENT_EXPORTS")
set_target_properties(mc_udp_client PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION})
target_link_libraries(mc_udp_client PUBLIC mc_udp_data)

install(
  TARGETS mc_udp_client
  EXPORT mc_udp_clientTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include/mc_udp
)

install(EXPORT mc_udp_clientTargets
  FILE mc_udp_clientTargets.cmake
  NAMESPACE mc_udp::
  DESTINATION lib/cmake/mc_udp
)


if(${BUILD_DUMMY_CLIENT})
  add_subdirectory(dummy)
endif()

if(${BUILD_MC_RTC_CLIENT})
  find_package(mc_rtc REQUIRED)
  find_package(Boost REQUIRED COMPONENTS program_options thread)

  set(MCUDPControl_SRC
    MCUDPControl.cpp
  )
  set(TARGET_NAME "MCUDPControlClient")
  link_directories(${Boost_LIBRARY_DIRS})
  add_executable(${TARGET_NAME} ${MCUDPControl_SRC})
  target_include_directories(${TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})
  target_link_libraries(${TARGET_NAME} mc_udp_client mc_rtc::mc_control ${Boost_LIBRARIES})
  set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME MCUDPControl)
  install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
endif()

