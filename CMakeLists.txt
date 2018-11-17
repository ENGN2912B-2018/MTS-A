cmake_minimum_required(VERSION 2.8)
project(main)

# add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -p -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(Boost 1.63.0 COMPONENTS filesystem iostreams system)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

# Also look for the CMakeLists.txt files in subdirectory src
include_directories(src)
add_subdirectory(src)

# Create the executable code for the main program from main.cpp
add_executable(mainExecutable main.cpp)

# Tell the linker to link the mainExecutable to headers in the mainLibrary the boost library
target_link_libraries(mainExecutable mainLibrary ${Boost_LIBRARIES})
