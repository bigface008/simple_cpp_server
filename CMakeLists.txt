cmake_minimum_required(VERSION 3.22)
project(MyCppServer)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



add_executable(client
    client.cpp
        util.h
        util.cpp)

add_executable(server
    server.cpp
        util.h
        util.cpp)
