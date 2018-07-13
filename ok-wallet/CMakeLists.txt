cmake_minimum_required(VERSION 3.4)
project(stellar-wallet)

if(${PROJECT_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	message(FATAL_ERROR "DO NOT run cmake in source directory")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_definitions(-DOKWALLET)

add_definitions(-DELPP_DISABLE_LOGS) #disable stellar-core log

enable_testing()

link_libraries(dl)
link_libraries(pthread)

include(ExternalProject)
add_subdirectory(src)

#FIXME: `make` will make testcase too, but `make test` does not work.
