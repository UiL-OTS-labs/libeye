
# Toolchain file to cross compile from host linux to target Windows 64-bit
# This file assumes you are on Linux and have installed the mingw-w64-*
# packages to cross compile c++ programs

#define the target.
set(CMAKE_SYSTEM_NAME Windows)

# I don't know for what CMAKE_SYSTEM_VERSION is used just taken from:
# http://www.vtk.org/Wiki/CMake_Cross_Compiling#The_toolchain_file
#set(CMAKE_SYSTEM_VERSION 1) 

# define the compilers
set(CMAKE_C_COMPILER    x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER  x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER   x86_64-w64-mingw32-windres)

set(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
