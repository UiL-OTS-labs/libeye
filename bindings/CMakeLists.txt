

option(PYTHON_BINDING "Build a language binding for pyton." OFF)


if (PYTHON_BINDING)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/python)
endif(PYTHON_BINDING)

