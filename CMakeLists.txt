

cmake_minimum_required(VERSION 2.6)
project(libeye CXX) 

#add versioning information
set (LIBEYE_VERSION_MAJOR 0)
set (LIBEYE_VERSION_MINOR 1)
set (LIBEYE_VERSION_MICRO 0)
set (LIBEYE_VERSION_STRING
     "\"jrep-${LIBEYE_VERSION_MAJOR}.${LIBEYE_VERSION_MINOR}.${LIBEYE_VERSION_MICRO}\""
    )

# setting of general options
option(BUILD_BINARIES "Build utilities that uses the library" ON)

# creation of a configure file
configure_file(
    ${PROJECT_SOURCE_DIR}/libeye-config.h.in
    ${PROJECT_BINARY_DIR}/libeye-config.h
    )

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/log")

#add the log library direcory
add_subdirectory(log)

#optionally add the binary directory
if (BUILD_BINARIES)
    add_subdirectory(bin)
endif(BUILD_BINARIES)

