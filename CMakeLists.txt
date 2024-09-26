cmake_minimum_required(VERSION 3.22)
project(MyCppServer)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(utils STATIC
        util.h
        util.cpp)
#target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)
link_libraries(utils)

add_executable(client client.cpp)
add_executable(server server.cpp
        Epoll.cpp
        Epoll.h
        InetAddress.cpp
        InetAddress.h
        Socket.cpp
        Socket.h)
