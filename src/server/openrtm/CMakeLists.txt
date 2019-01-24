pkg_check_modules(HRPSYS_BASE hrpsys-base)

if(${HRPSYS_BASE_FOUND})
  option(BUILD_MCNNGControl "Build MCNNGControl" ON)
  if(${BUILD_MCNNGControl})
    set(MCNNGControl_lib_SRC
      MCNNGControl.cpp
      ${server_SRC}
    )
    set(MCNNGControl_lib_HDR
      MCNNGControl.h
      ${server_HDR}
    )
    add_library(MCNNGControl SHARED ${MCNNGControl_lib_SRC} ${MCNNGControl_lib_HDR})
    target_include_directories(MCNNGControl PUBLIC ${HRPSYS_BASE_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(MCNNGControl nng ${HRPSYS_BASE_LIBRARIES})
    set_target_properties(MCNNGControl PROPERTIES PREFIX "")

    add_executable(MCNNGControlComp MCNNGControlComp.cpp)
    target_link_libraries(MCNNGControlComp MCNNGControl)

    install(TARGETS MCNNGControl MCNNGControlComp
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
    )
  endif()
endif()