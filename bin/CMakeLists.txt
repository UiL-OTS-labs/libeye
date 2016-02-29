# only install the binaries linked against the dynamic libs
# targets linking to the static libs are only to see whether
# static linking succeeds.
SET (INSTALL_BINARIES readeyelog testwritelog)

add_executable(readeyelog readeyelog.cpp)
add_executable(testwritelog testwritelog.cpp)

add_executable(readeyelog-static readeyelog.cpp)
add_executable(testwritelog-static testwritelog.cpp)

# Don't forget to define BUILDER_STATIC_DEFINE while linking against the 
# the static library.
set_target_properties(readeyelog-static   PROPERTIES COMPILE_DEFINITIONS EYELOG_STATIC_DEFINE)
set_target_properties(testwritelog-static PROPERTIES COMPILE_DEFINITIONS EYELOG_STATIC_DEFINE)
#everyone needs c++11 
set_property(TARGET readeyelog PROPERTY CXX_STANDARD 11)
set_property(TARGET readeyelog PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET testwritelog PROPERTY CXX_STANDARD 11)
set_property(TARGET testwritelog PROPERTY CXX_STANDARD_REQUIRED ON)

# build the programs that link the dynamic library
target_link_libraries(readeyelog ${EYELOG_SHARED_LIB})
target_link_libraries(testwritelog ${EYELOG_SHARED_LIB})

target_link_libraries(readeyelog-static ${EYELOG_STATIC_LIB})
target_link_libraries(testwritelog-static ${EYELOG_STATIC_LIB})

INSTALL(TARGETS ${INSTALL_BINARIES} DESTINATION bin)
