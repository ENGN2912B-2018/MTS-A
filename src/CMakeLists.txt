cmake_minimum_required(VERSION 2.8)

project(mainLibrary)

# Specify the compiler and version of c++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# add server.hpp and server.cpp to the mainLibrary
add_library(mainLibrary image/image.h
            client/client.h client/client.cpp
            server/server.h server/server.cpp)

# Set the link properties of this library
set_target_properties(mainLibrary PROPERTIES LINKER_LANGUAGE CXX)
