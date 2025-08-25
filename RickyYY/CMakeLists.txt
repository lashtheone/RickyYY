cmake_minimum_required(VERSION 3.16)
project(min_chat LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simple_server
  src/server.cpp
  src/common.cpp
)
add_executable(simple_client
  src/client.cpp
  src/common.cpp
)

target_include_directories(simple_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(simple_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
