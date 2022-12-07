cmake_minimum_required(VERSION 3.0)

project("ALT Diagnostic tool")

if(UNIX)
  include(GNUInstallDirs)
else()
  if (WIN32)
    set(${CMAKE_INSTALL_LIBDIR} "lib")
    set(${CMAKE_INSTALL_DATADIR} "share")
    set(${CMAKE_INSTALL_INCLUDEDIR} "include")
    set(${CMAKE_INSTALL_BINDIR} "bin")
    message(STATUS "Setting installation destination on Windows to: ${CMAKE_INSTALL_PREFIX}")
  else()
    message(FATAL_ERROR "System not UNIX nor WIN32 - not implemented yet")
  endif()
endif()

add_subdirectory(src)

