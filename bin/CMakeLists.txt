

add_executable(readeyelog readeyelog.cpp)

#everyone needs c++11 
set_property(TARGET readeyelog PROPERTY CXX_STANDARD 11)
set_property(TARGET readeyelog PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(readeyelog eyelog)
