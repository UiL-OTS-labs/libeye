

# source files to build the library.
set (BASE_SOURCES
        #BaseString.cpp
        #DArray.cpp
        #Instantation.cpp
#        Atomic.cpp
        SharedPtr.cpp
        )

# headers to build the library
set (BASE_HEADERS
        #BaseString.h
        #DArray.h
#        Atomic.h
        SharedPtr.h
        )

# headers needed to use the library.
set (BASE_INSTALL_HEADERS 
        #DArray.h
        #BaseString.h
        #Atomic.h
        SharedPtr.h
        )

add_library(${BASE_SHARED_LIB} SHARED ${BASE_SOURCES} ${BASE_HEADERS})
add_library(${BASE_STATIC_LIB} STATIC ${BASE_SOURCES} ${BASE_HEADERS})

generate_export_header(${BASE_SHARED_LIB}
                       BASE_NAME base
                       )

set_target_properties(${BASE_SHARED_LIB}
    PROPERTIES COMPILE_FLAGS -DBUILD_BASE_SHARED
    )
set_target_properties(${BASE_STATIC_LIB}
    PROPERTIES COMPILE_FLAGS -DBASE_STATIC_DEFINE
    )

#add compiler features
set_property(TARGET ${BASE_SHARED_LIB} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${BASE_SHARED_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET ${BASE_STATIC_LIB} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${BASE_STATIC_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

INSTALL(TARGETS ${BASE_SHARED_LIB}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

INSTALL(TARGETS ${BASE_STATIC_LIB}
        ARCHIVE
        DESTINATION lib
        )

INSTALL(FILES ${INSTALL_HEADERS}
        DESTINATION ${INSTALL_HEADER_DIR}/base
        ) 

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/base_export.h
        DESTINATION ${INSTALL_HEADER_DIR}/base
        )
