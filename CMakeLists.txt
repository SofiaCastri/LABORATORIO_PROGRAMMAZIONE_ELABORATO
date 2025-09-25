cmake_minimum_required(VERSION 4.0)
project(LABORATORIO_PROGRAMMAZIONE_ELABORATO)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(test)

set(SOURCE_FILES Transaction.cpp Transaction.h Account.cpp Account.h)
add_executable(LABORATORIO_PROGRAMMAZIONE_ELABORATO main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(LABORATORIO_PROGRAMMAZIONE_ELABORATO core)
