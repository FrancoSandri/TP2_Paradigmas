cmake_minimum_required(VERSION 3.10)

project(ej3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ej3 main.cpp)

target_compile_options(ej3 PRIVATE -Wall -Wextra -Wpedantic)
find_package(Threads REQUIRED)
target_link_libraries(ej3 Threads::Threads)
