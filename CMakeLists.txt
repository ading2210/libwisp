cmake_minimum_required(VERSION 3.25)

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