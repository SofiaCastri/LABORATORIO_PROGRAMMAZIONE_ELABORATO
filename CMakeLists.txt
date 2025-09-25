cmake_minimum_required(VERSION 4.0)
project(LABORATORIO_PROGRAMMAZIONE_ELABORATO)

set(CMAKE_CXX_STANDARD 20)

add_executable(LABORATORIO_PROGRAMMAZIONE_ELABORATO main.cpp
        Transaction.cpp
        Transaction.h
        Account.cpp
        Account.h)
