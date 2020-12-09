#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

set(include_DIR "${PROJECT_SOURCE_DIR}/include/mc_udp")

set(data_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/data/RobotSensors.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/data/RobotControl.cpp"
)

set(data_HDR
  "${include_DIR}/data/api.h"
  "${include_DIR}/data/MultiRobotMessage.h"
  "${include_DIR}/data/MultiRobotMessage.hpp"
  "${include_DIR}/data/RobotSensors.h"
  "${include_DIR}/data/RobotControl.h"
  "${include_DIR}/data/utils.h"
  "${include_DIR}/logging.h"
)

add_library(mc_udp_data SHARED ${data_SRC} ${data_HDR})
add_library(mc_udp::data ALIAS mc_udp_data)
set_target_properties(mc_udp_data PROPERTIES EXPORT_NAME data)
set_target_properties(mc_udp_data PROPERTIES COMPILE_FLAGS "-DMC_UDP_DATA_EXPORTS")
set_target_properties(mc_udp_data PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION})
target_include_directories(mc_udp_data PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_subdirectory(client)
add_subdirectory(server)

install(DIRECTORY "${include_DIR}/data" DESTINATION include/mc_udp
        FILES_MATCHING PATTERN "*.h")
install(FILES "${include_DIR}/logging.h" DESTINATION include/mc_udp)

install(
  TARGETS mc_udp_data
  EXPORT mc_udpTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include/mc_udp
)

install(EXPORT mc_udpTargets
  FILE mc_udpTargets.cmake
  NAMESPACE mc_udp::
  DESTINATION lib/cmake/mc_udp
)

