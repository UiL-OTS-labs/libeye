# Running unit tests

set(CXX_TEST_PATH)

# headers to build the library
find_package(CxxTest )
if(CXXTEST_FOUND)
else()
    get_filename_component(CXX_TEST_PATH DIRECTORY)
    set(CMAKE_PREFIX_PATH ${CXX_TEST_PATH})
endif()

# try once more
if (!CXXTEST_FOUND)
    find_package(CxxTest)
endif()

if(CXXTEST_FOUND)
    option(BUILD_UNIT_TESTS "Build unit tests" ON)
    if (BUILD_UNIT_TESTS)
        set (TEST_HEADERS)
        set (TEST_SOURCES)
        set (ALL_UNIT_TEST_SOURCES)

        # output that includes the main function of the unit test.
        set (TEST_MAIN "${CMAKE_CURRENT_BINARY_DIR}/UnitTest.cpp")
        # output name of the unit test executable
        set (UNIT_TEST unit-test)

        file(GLOB TEST_HEADERS  *.h)

        foreach(fn IN LISTS TEST_HEADERS)
            set (fnout "")
            set (current_filename "")
            string(REPLACE ".h" ".cpp" fnout ${fn})
            get_filename_component(current_filename ${fnout} NAME)
            set (fnout "${CMAKE_CURRENT_BINARY_DIR}/${current_filename}")
            #message(STATUS "cxxtestgen is creating \"${fnout}\"")
            add_custom_command (OUTPUT ${fnout} 
                                COMMAND cxxtestgen -o ${fnout}  --part --error-printer ${fn}
                                DEPENDS ${fn}                        
                                )
            list (APPEND TEST_SOURCES ${fnout})
        endforeach(fn)

        add_custom_command (OUTPUT ${TEST_MAIN}
                            COMMAND cxxtestgen --error-printer --root -o ${TEST_MAIN}
                            )

        list(APPEND ALL_UNIT_TEST_SOURCES ${TEST_SOURCES} ${TEST_MAIN})

        set_source_files_properties (${ALL_UNIT_TEST_SOURCES} PROPERTIES GENERATED TRUE)

        add_custom_target(generated_files DEPENDS ${ALL_UNIT_TEST_SOURCES})
        add_executable (${UNIT_TEST} ${ALL_UNIT_TEST_SOURCES})
        add_dependencies (${UNIT_TEST} generated_files)


        if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
            ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
            target_compile_options(${UNIT_TEST} PUBLIC -pthread)
            target_link_libraries(${UNIT_TEST} pthread)
        endif()

        set_property(TARGET ${UNIT_TEST} PROPERTY CXX_STANDARD 11)
        set_property(TARGET ${UNIT_TEST} PROPERTY CXX_STANDARD_REQUIRED ON)

        target_link_libraries(${UNIT_TEST} ${EYELOG_SHARED_LIB})
        target_link_libraries(${UNIT_TEST} ${BASE_SHARED_LIB})
    endif()
else()
endif()
