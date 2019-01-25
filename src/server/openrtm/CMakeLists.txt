pkg_check_modules(HRPSYS_BASE hrpsys-base)

if(${HRPSYS_BASE_FOUND})
  option(BUILD_Server_OpenRTM "Build OpenRTM server component" ON)
  if(${BUILD_Server_OpenRTM})
    set(MCNNGControl_lib_SRC
      MCNNGControl.cpp
      ${server_SRC}
    )
    set(MCNNGControl_lib_HDR
      MCNNGControl.h
      ${server_HDR}
    )
    link_directories(MCNNGControl ${HRPSYS_BASE_LIBRARY_DIRS})
    add_library(MCNNGControl SHARED ${MCNNGControl_lib_SRC} ${MCNNGControl_lib_HDR})
    target_include_directories(MCNNGControl PUBLIC ${HRPSYS_BASE_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(MCNNGControl nng ${HRPSYS_BASE_LIBRARIES})
    set_target_properties(MCNNGControl PROPERTIES PREFIX "")

    add_executable(MCNNGControlComp MCNNGControlComp.cpp)
    target_link_libraries(MCNNGControlComp MCNNGControl)

    install(TARGETS MCNNGControl MCNNGControlComp
      RUNTIME DESTINATION "${HRPSYS_BASE_PREFIX}/bin"
      LIBRARY DESTINATION "${HRPSYS_BASE_PREFIX}/lib"
      ARCHIVE DESTINATION "${HRPSYS_BASE_PREFIX}/lib"
    )
  endif()
endif()
