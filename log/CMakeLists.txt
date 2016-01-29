
add_library(eyelog SHARED
            EyeLog.cpp
            EyeLog.h
            )
            
add_library(eyelog-static STATIC 
            EyeLog.cpp
            EyeLog.h
            )

#add compiler features
set_property(TARGET eyelog PROPERTY CXX_STANDARD 11)
set_property(TARGET eyelog PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET eyelog-static PROPERTY CXX_STANDARD 11)
set_property(TARGET eyelog-static PROPERTY CXX_STANDARD_REQUIRED ON)
