

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
target_link_libraries(readeyelog eyelog)
target_link_libraries(testwritelog eyelog)

target_link_libraries(readeyelog-static eyelog-static)
target_link_libraries(testwritelog-static eyelog-static)
