
set (EYELOG_SHARED_LIB
     eyelog_${LIBEYE_VERSION_MAJOR}_${LIBEYE_VERSION_MINOR}
     )

set (EYELOG_STATIC_LIB
     eyelog_${LIBEYE_VERSION_MAJOR}_${LIBEYE_VERSION_MINOR}-static
     )

# source files to build the library.
set (LOG_SOURCES
        PEyeLog.cpp
        PEyeLogEntry.cpp
        PExperiment.cpp
        PCoordinate.cpp
        cEyeLog.cpp
        cError.cpp
        )

# headers to build the library
set (LOG_HEADERS
        PEyeLog.h
        PEyeLogEntry.h
        PExperiment.h
        PCoordinate.h
        cEyeLog.h
        cError.h
        constants.h
        Shapes.h
        )

# headers needed to use the library.
set (INSTALL_HEADERS 
        cEyeLog.h
        cError.h
        constants.h
        Shapes.h
        EyeLog.h
        )


add_library(${EYELOG_SHARED_LIB} SHARED ${LOG_SOURCES} ${LOG_HEADERS})
add_library(${EYELOG_STATIC_LIB} STATIC ${LOG_SOURCES} ${LOG_HEADERS})

#generate_export_header(eyelog)
generate_export_header(${EYELOG_SHARED_LIB}
                       BASE_NAME eyelog
                       )
#generate_export_header(eyelog-static)

set_target_properties(${EYELOG_SHARED_LIB}
    PROPERTIES COMPILE_FLAGS -DBUILD_EYELOG_SHARED
    )
set_target_properties(${EYELOG_STATIC_LIB}
    PROPERTIES COMPILE_FLAGS -DEYELOG_STATIC_DEFINE
    )

#add compiler features
set_property(TARGET ${EYELOG_SHARED_LIB} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${EYELOG_SHARED_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET ${EYELOG_STATIC_LIB} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${EYELOG_STATIC_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

INSTALL(TARGETS ${EYELOG_SHARED_LIB}
        LIBRARY
        DESTINATION lib)

INSTALL(TARGETS ${EYELOG_STATIC_LIB}
        ARCHIVE
        DESTINATION lib)

INSTALL(FILES ${INSTALL_HEADERS}
        DESTINATION ${INSTALL_HEADER_DIR}
        ) 
