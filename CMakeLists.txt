cmake_minimum_required(VERSION 3.22)
project(MyCppServer)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(utils STATIC
        src/util.h
        src/util.cpp)
#target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)
link_libraries(utils)

add_executable(client client.cpp)
add_executable(server server.cpp
        src/Epoll.cpp
        src/Epoll.h
        src/InetAddress.cpp
        src/InetAddress.h
        src/Socket.cpp
        src/Socket.h
        src/Channel.cpp
        src/Channel.h
        src/Server.cpp
        src/Server.h
        src/EventLoop.cpp
        src/EventLoop.h)
