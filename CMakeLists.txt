cmake_minimum_required(VERSION 3.25)
include(ExternalProject)

project(
  libwisp
  VERSION 0.0.1
  LANGUAGES CXX
)

set(
  WISP_HEADERS
  wisp/buffer.hpp
  wisp/packet.hpp
  wisp/client.hpp
)

set (
  WISP_SOURCES
  wisp/buffer.cpp
  wisp/packet.cpp
  wisp/client.cpp
)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/prefix/lib)

add_library(
  wisp
  ${WISP_HEADERS}
  ${WISP_SOURCES}
)

add_executable(
  sampleclient
  examples/sampleclient.cpp
)

ExternalProject_Add(
  IXWebSocket
  GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/prefix
    -DUSE_TLS=1
    -DBUILD_SHARED_LIBS=1
)
include_directories(wisp ${CMAKE_CURRENT_BINARY_DIR}/prefix/include)
include_directories(sampleclient ${CMAKE_CURRENT_BINARY_DIR}/prefix/include)
include_directories(sampleclient ${CMAKE_CURRENT_SOURCE_DIR})
add_dependencies(wisp IXWebSocket)
add_dependencies(sampleclient wisp)

set_target_properties(wisp PROPERTIES PUBLIC_HEADER "${WISP_HEADERS}")
find_library(
  ixwebsocket 
  NAMES ixwebsocket
  PATHS ${CMAKE_CURRENT_BINARY_DIR}/prefix/lib
  REQUIRED
)
target_link_libraries(
  sampleclient 
  PRIVATE wisp
  PRIVATE ixwebsocket
)

INSTALL(
  TARGETS wisp 
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/wisp
)