

# source files to build the library.
set (LOG_SOURCES
        BaseString.cpp
        DArray.cpp
        Instantation.cpp
        PEyeLog.cpp
        PEyeLogEntry.cpp
        PExperiment.cpp
        PCoordinate.cpp
        #cEyeLog.cpp
        cError.cpp
        )

# headers to build the library
set (LOG_HEADERS
        BaseString.h
        constants.h
        DArray.h
        Instantation.h
        PEyeLog.h
        PEyeLogEntry.h
        PExperiment.h
        PCoordinate.h
        cEyeLog.h
        cError.h
        Shapes.h
        TypeDefs.h
        )

# headers needed to use the library.
set (INSTALL_HEADERS 
        cEyeLog.h
        cError.h
        constants.h
        Shapes.h
        DArray.h
        EyeLog.h
        PEyeLogEntry.h
        TypeDefs.h
        BaseString.h
        PCoordinate.h
        PExperiment.h
        PEyeLog.h
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
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

INSTALL(TARGETS ${EYELOG_STATIC_LIB}
        ARCHIVE
        DESTINATION lib
        )

INSTALL(FILES ${INSTALL_HEADERS}
        DESTINATION ${INSTALL_HEADER_DIR}/eyelog
        ) 

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/eyelog_export.h
        DESTINATION ${INSTALL_HEADER_DIR}/eyelog
        )
