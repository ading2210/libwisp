cmake_minimum_required(VERSION 3.25)
include(ExternalProject)

project(
  wisp-client-cpp
  VERSION 0.0.1
  LANGUAGES CXX
)

add_library(
  wispclient
  src/wispbuffer.cpp
  src/wispbuffer.hpp
  src/wisppacket.cpp
  src/wisppacket.hpp
  src/wispconnection.cpp
  src/wispconnection.hpp
)

ExternalProject_Add(
  IXWebSocket
  GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
  UPDATE_DISCONNECTED TRUE
  STEP_TARGETS build
  CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/prefix
    -DUSE_TLS=1
)
add_dependencies(wispclient IXWebSocket-build)
include_directories(wispclient ${CMAKE_CURRENT_BINARY_DIR}/prefix/include)
