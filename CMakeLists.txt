cmake_minimum_required(VERSION 3.19)
project (IMP2023 LANGUAGES CXX DESCRIPTION "Image Processing 2023 projects")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -mtune=native -march=native")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
	set(CMAKE_CXX_FLAGS_RelWithDebInfo "${CMAKE_CXX_FLAGS} -O2 -g")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(test test.cpp)
target_link_libraries(test ${OpenCV_LIBS})

add_executable(filter-basic filter-basic.cpp)
target_link_libraries(filter-basic ${OpenCV_LIBS})

add_executable(color color.cpp)
target_link_libraries(color ${OpenCV_LIBS})

add_executable(blkproc blkproc.cpp)
target_link_libraries(blkproc ${OpenCV_LIBS})