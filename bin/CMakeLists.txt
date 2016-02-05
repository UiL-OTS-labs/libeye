

add_executable(readeyelog readeyelog.cpp)
add_executable(testwritelog testwritelog.cpp)

#everyone needs c++11 
set_property(TARGET readeyelog PROPERTY CXX_STANDARD 11)
set_property(TARGET readeyelog PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET testwritelog PROPERTY CXX_STANDARD 11)
set_property(TARGET testwritelog PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(readeyelog eyelog-static)
target_link_libraries(testwritelog eyelog-static)
