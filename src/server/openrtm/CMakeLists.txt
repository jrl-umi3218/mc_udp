pkg_check_modules(HRPSYS_BASE hrpsys-base)

if(${HRPSYS_BASE_FOUND})
  option(BUILD_Server_OpenRTM "Build OpenRTM server component" ON)
  if(${BUILD_Server_OpenRTM})
    set(MCUDPControl_lib_SRC
      MCUDPControl.cpp
      ${server_SRC}
    )
    set(MCUDPControl_lib_HDR
      MCUDPControl.h
      ${server_HDR}
    )
    link_directories(MCUDPControl ${HRPSYS_BASE_LIBRARY_DIRS})
    add_library(MCUDPControl SHARED ${MCUDPControl_lib_SRC} ${MCUDPControl_lib_HDR})
    target_include_directories(MCUDPControl PUBLIC ${HRPSYS_BASE_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(MCUDPControl ${HRPSYS_BASE_LIBRARIES})
    set_target_properties(MCUDPControl PROPERTIES PREFIX "")

    add_executable(MCUDPControlComp MCUDPControlComp.cpp)
    target_link_libraries(MCUDPControlComp MCUDPControl)

    install(TARGETS MCUDPControl MCUDPControlComp
      RUNTIME DESTINATION "${HRPSYS_BASE_PREFIX}/bin"
      LIBRARY DESTINATION "${HRPSYS_BASE_PREFIX}/lib"
      ARCHIVE DESTINATION "${HRPSYS_BASE_PREFIX}/lib"
    )
  endif()
endif()
