
# source files to build the library.
set (LOG_SOURCES
        PEyeLog.cpp
        PEyeLogEntry.cpp
        PCoordinate.cpp
        cEyeLog.cpp
        )

# headers to build the library
set (LOG_HEADERS
        PEyeLog.h
        PEyeLogEntry.h
        PCoordinate.h
        cEyeLog.h
        )

# headers needed to use the library.
set (INSTALL_HEADERS cEyeLog.h EyeLog.h)
INSTALL(FILES ${INSTALL_HEADERS} DESTINATION include)

add_library(eyelog SHARED ${LOG_SOURCES} ${LOG_HEADERS})
add_library(eyelog-static STATIC ${LOG_SOURCES} ${LOG_HEADERS})

generate_export_header(eyelog)
#generate_export_header(eyelog-static)

set_target_properties(eyelog PROPERTIES COMPILE_FLAGS -DBUILD_EYELOG_SHARED)
set_target_properties(eyelog-static PROPERTIES COMPILE_FLAGS -DEYELOG_STATIC_DEFINE)

#add compiler features
set_property(TARGET eyelog PROPERTY CXX_STANDARD 11)
set_property(TARGET eyelog PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET eyelog-static PROPERTY CXX_STANDARD 11)
set_property(TARGET eyelog-static PROPERTY CXX_STANDARD_REQUIRED ON)
