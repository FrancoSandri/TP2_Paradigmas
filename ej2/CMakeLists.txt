cmake_minimum_required(VERSION 3.10)

project(ej2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(headers)

file(GLOB SRC_FILES "src/*.cpp")
file(GLOB MAIN_FILES "main/*.cpp")

add_executable(ej2 ${SRC_FILES} ${MAIN_FILES})

find_package(Threads REQUIRED)
target_link_libraries(ej2 Threads::Threads)
target_compile_options(ej2 PRIVATE -Wall -Wextra -Wpedantic)
