# libeye
A small library that helps processing eye movement data

libeye has a dependency on CMake in order to install the library. This is a very
short explanation of how to start building the library. Create an empty directory
and change to that directory. Type cmake <path to libeye> -DCMAKE_BUILD_TYPE=Release
Replace Release by Debug for a debug build.

directory structure:

--libeye            libeye root directory
    |
    |-- log         This contains the logging facilities
    |
    |-- bin         This constains the sources for the executabels
    |
    |-- bindings    This directory contains binding to other languages
    |
    |-- samples     This directory contains some test sample for the library.
