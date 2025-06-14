cmake_minimum_required(VERSION 3.10)

project(ej1)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir los headers
include_directories(headers)

# Archivos fuente
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB MAIN_FILES "main/*.cpp")

# Crear ejecutable
add_executable(ej1 ${SRC_FILES} ${MAIN_FILES})