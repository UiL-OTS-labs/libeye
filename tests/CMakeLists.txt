# Running unit tests

set(CXX_TEST_PATH "" CACHE PATH "Add path to CxxTest utility.")

# headers to build the library
if(CXX_TEST_PATH)
##    get_filename_component(CXX_TEST_PATH DIRECTORY)
    set(CMAKE_PREFIX_PATH   ${CXX_TEST_PATH}/bin)
	#set(${CMAKE_INCLUDE_PATH} "${CMAKE_INCLUDE_PATH} ${CXX_TEST_PATH}/cxxtest")
	set(CXXTEST_INCLUDE_DIR "${CXX_TEST_PATH}")
endif()

find_package(CxxTest )

if(CXXTEST_FOUND)
    option(BUILD_UNIT_TESTS "Build unit tests" ON)
    if (BUILD_UNIT_TESTS)
	    include_directories(${CXXTEST_INCLUDE_DIR})
        set (TEST_HEADERS)
        set (TEST_SOURCES)
        set (ALL_UNIT_TEST_SOURCES)

#        # output that includes the main function of the unit test.
        set (TEST_MAIN "${CMAKE_CURRENT_BINARY_DIR}/UnitTest.cpp")
        # output name of the unit test executable
        set (UNIT_TEST unit-test)
		set (UNIT_TEST_GENERATED unittest_generated_files)

        file(GLOB TEST_HEADERS  *.h)

        foreach(fn IN LISTS TEST_HEADERS)
            set (fnout "")
            set (current_filename "")
            string(REPLACE ".h" ".cpp" fnout ${fn})
            get_filename_component(current_filename ${fnout} NAME)
            set (fnout "${CMAKE_CURRENT_BINARY_DIR}/${current_filename}")
            message(STATUS "cxxtestgen is creating \"${fnout}\"")
            add_custom_command (OUTPUT ${fnout} 
                                COMMAND ${CXXTEST_TESTGEN_EXECUTABLE} -o ${fnout}  --part --error-printer ${fn}
                                DEPENDS ${fn}                        
                                )
            list (APPEND TEST_SOURCES ${fnout})
        endforeach(fn)

        add_custom_command (OUTPUT ${TEST_MAIN}
                            COMMAND ${CXXTEST_TESTGEN_EXECUTABLE} --error-printer --root -o ${TEST_MAIN}
                            )
        message(STATUS "cxxtestgen is creating \"${TEST_MAIN}\"")

        list(APPEND ALL_UNIT_TEST_SOURCES ${TEST_SOURCES})
        list(APPEND ALL_UNIT_TEST_SOURCES ${TEST_MAIN})

        set_source_files_properties (${ALL_UNIT_TEST_SOURCES} PROPERTIES GENERATED TRUE)

        add_custom_target(${UNIT_TEST_GENERATED} DEPENDS ${ALL_UNIT_TEST_SOURCES})
        add_executable (${UNIT_TEST} ${ALL_UNIT_TEST_SOURCES})
        add_dependencies (${UNIT_TEST} ${UNIT_TEST_GENERATED})

        if (${CMAKE_MAJOR_VERSION} GREATER "3" AND
            ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
            ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
            target_compile_options(${UNIT_TEST} PUBLIC -pthread)
            target_link_libraries(${UNIT_TEST} pthread)
            message(STATUS "using target compile options\"${fnout}\"")
        elseif(${CMAKE_MAJOR_VERSION} LESS "3" AND
            ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
            ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
            set_target_properties(${UNIT_TEST}
                                  PROPERTIES
                                  COMPILE_FLAGS "-pthread"
                                  LINK_FLAGS "-pthread"
                                  )
            message(STATUS "target properties")
        endif()

        set_property(TARGET ${UNIT_TEST} PROPERTY CXX_STANDARD 11)
        set_property(TARGET ${UNIT_TEST} PROPERTY CXX_STANDARD_REQUIRED ON)

        target_link_libraries(${UNIT_TEST} ${EYELOG_SHARED_LIB})
        target_link_libraries(${UNIT_TEST} ${BASE_SHARED_LIB})
    endif()
else()
endif()
