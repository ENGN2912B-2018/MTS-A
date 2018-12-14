cmake_minimum_required(VERSION 2.8)
project(main)

# add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -p -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# find Boost and Qt5Widgets
find_package(Boost 1.63.0 COMPONENTS filesystem iostreams system)
find_package(Qt5Widgets REQUIRED)

# include boost
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# include Qt5Widges (which includes QtCore & QtGui),
# based on CMake from: https://www.kdab.com/using-cmake-with-qt-5/
if(Qt5Widgets_FOUND)
  include_directories(${Qt5Widgets_INCLUDES})
  add_definitions(${Qt5Widgets_DEFINITIONS})

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
endif()

# Also look for the CMakeLists.txt files in subdirectory src
include_directories(src)
add_subdirectory(src)

# Create the executables
add_executable(mainExecutable main.cpp)
add_executable(client client.cpp)
add_executable(server server.cpp)

# Tell the linker to link the executables to the mainLibrary, Boost, and Qt5
target_link_libraries(mainExecutable mainLibrary ${Boost_LIBRARIES} ${Qt5Widgets_LIBRARIES})
target_link_libraries(server mainLibrary ${Boost_LIBRARIES} ${Qt5Widgets_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES} ${Qt5Widgets_LIBRARIES} mainLibrary)
