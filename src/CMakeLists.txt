set(include_DIR "${PROJECT_SOURCE_DIR}/include/mc_udp")

set(data_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/data/RobotSensors.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/data/RobotControl.cpp"
)

set(data_HDR
  "${include_DIR}/data/RobotSensors.h"
  "${include_DIR}/data/RobotControl.h"
  "${include_DIR}/logging.h"
)

add_subdirectory(client)
add_subdirectory(server)
