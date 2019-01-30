set(client_HDR
  "${include_DIR}/client/Client.h"
  ${data_HDR}
)

set(client_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/Client.cpp"
  ${data_SRC}
)

add_subdirectory(dummy)

find_package(mc_rtc)
if(${mc_rtc_FOUND})
  option(BUILD_CLIENT "Build client" ON)
  find_package(Boost REQUIRED COMPONENTS program_options)
  if(${BUILD_CLIENT})
    set(MCUDPControl_SRC
      ${client_SRC}
      ${client_HDR}
      MCUDPControl.cpp
    )
    set(TARGET_NAME "MCUDPControlClient")
    link_directories(${Boost_LIBRARY_DIRS})
    add_executable(${TARGET_NAME} ${MCUDPControl_SRC})
    target_include_directories(${TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} mc_rtc::mc_control ${Boost_LIBRARIES})
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME MCUDPControl)
    install(TARGETS ${TARGET_NAME}
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
    )
  endif()
endif()
