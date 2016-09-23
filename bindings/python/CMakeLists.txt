

find_program(PYTHON "python")

set (SETUP_PY_IN    "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
set (SETUP_PY       "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set (PYEYE_DEPS     "${EYELOG_SHARED_LIB}")
set (OUTPUT         "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

configure_file(${SETUP_PY_IN} ${SETUP_PY})

add_custom_command( OUTPUT ${OUTPUT}
                    COMMAND ${PYTHON} ${SETUP_PY} build
                    COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                    DEPENDS ${DEPS}
                    )

add_custom_target(pyeye ALL DEPENDS ${OUTPUT})

