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
  set(MCControlNNG_SRC
    ${client_SRC}
    ${client_HDR}
    MCControlNNG.cpp
  )
  add_executable(MCControlNNG ${MCControlNNG_SRC})
  target_include_directories(MCControlNNG PRIVATE "${PROJECT_SOURCE_DIR}/include")
  target_link_libraries(MCControlNNG mc_rtc::mc_control nng)

  install(TARGETS MCControlNNG
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
endif()
