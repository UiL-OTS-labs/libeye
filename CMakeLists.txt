

cmake_minimum_required(VERSION 2.8)
project(libeye CXX) 

include (GenerateExportHeader)

#add versioning information
set (LIBEYE_VERSION_MAJOR 0)
set (LIBEYE_VERSION_MINOR 1)
set (LIBEYE_VERSION_MICRO 0)
set (LIBEYE_VERSION_STRING
     "\"jrep-${LIBEYE_VERSION_MAJOR}.${LIBEYE_VERSION_MINOR}.${LIBEYE_VERSION_MICRO}\""
    )

# location where to install the headers.
SET(INSTALL_HEADER_DIR
    include/${CMAKE_PROJECT_NAME}-${LIBEYE_VERSION_MAJOR}.${LIBEYE_VERSION_MINOR}
    )

# library names
set (EYELOG_SHARED_LIB
     eyelog_${LIBEYE_VERSION_MAJOR}_${LIBEYE_VERSION_MINOR}
     )
set (EYELOG_STATIC_LIB
     eyelog_${LIBEYE_VERSION_MAJOR}_${LIBEYE_VERSION_MINOR}-static
     )


if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS ${CXX_FLAGS} "-std=c++0x -Wall -Wextra -pedantic")
endif()

# setting of general options
option(BUILD_BINARIES "Build utilities that uses the library" ON)

# creation of a configure file
configure_file(
    ${PROJECT_SOURCE_DIR}/libeye-config.h.in
    ${PROJECT_BINARY_DIR}/libeye-config.h
    )

# install headers
INSTALL(FILES
        ${PROJECT_BINARY_DIR}/libeye-config.h
        DESTINATION ${INSTALL_HEADER_DIR}
        ) 

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/log")
include_directories("${CMAKE_BINARY_DIR}/log")

#add the log library direcory
add_subdirectory(log)
#add the language bindings
add_subdirectory(bindings)

#optionally add the binary directory
if (BUILD_BINARIES)
    add_subdirectory(bin)
endif(BUILD_BINARIES)

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENCE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${LIBEYE_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${LIBEYE_VERSION_MINOR}")

option(BUILD_LIBEYE_PACKAGE OFF)
if(BUILD_LIBEYE_PACKAGE)
    option(BUILD_DEB_PKG OFF)
endif(BUILD_LIBEYE_PACKAGE)

if(BUILD_DEB_PKG)
    SET(CPACK_GENERATOR "DEB" )
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maarten Duijndam")
endif(BUILD_DEB_PKG)

include(CPack)
