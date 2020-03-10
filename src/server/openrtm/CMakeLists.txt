#
# Copyright 2019-2020 CNRS-UM LIRMM, CNRS-AIST JRL
#

pkg_check_modules(HRPSYS_BASE hrpsys-base)

macro(build_rtc_component NAME)
  set(${NAME}_lib_SRC
    ${NAME}.cpp
  )
  set(${NAME}_lib_HDR
    ${NAME}.h
  )
  link_directories(${NAME} ${HRPSYS_BASE_LIBRARY_DIRS})
  add_library(${NAME} SHARED ${${NAME}_lib_SRC} ${${NAME}_lib_HDR})
  target_include_directories(${NAME} PUBLIC ${HRPSYS_BASE_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")
  target_link_libraries(${NAME} PUBLIC ${HRPSYS_BASE_LIBRARIES} mc_udp::data mc_udp::server)
  set_target_properties(${NAME} PROPERTIES PREFIX "")
  if(OpenRTM_LEGACY)
    target_compile_definitions(${NAME} PUBLIC MC_UDP_OPENRTM_LEGACY)
  endif()

  add_executable(${NAME}Comp ${NAME}Comp.cpp)
  target_link_libraries(${NAME}Comp ${NAME})

  install(TARGETS ${NAME} ${NAME}Comp
    RUNTIME DESTINATION "${HRPSYS_BASE_PREFIX}/bin"
    LIBRARY DESTINATION "${HRPSYS_BASE_PREFIX}/lib"
    ARCHIVE DESTINATION "${HRPSYS_BASE_PREFIX}/lib"
  )
endmacro()

if(${HRPSYS_BASE_FOUND})
  option(OpenRTM_LEGACY "Use TimedDoubleSeq for every port of MCUDPSensors" OFF)
  if(${BUILD_OPENRTM_SERVER})
    build_rtc_component(MCUDPSensors)
    build_rtc_component(MCUDPControl)
  endif()
endif()
