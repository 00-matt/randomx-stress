cmake_minimum_required(VERSION 3.13)
project(randomx-stress LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(RandomX REQUIRED)
find_package(Threads REQUIRED)

add_executable(randomx-stress main.cc)
set_target_properties(randomx-stress PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_link_libraries(randomx-stress RandomX::RandomX Threads::Threads)
