
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

# Set file properties to C++ and dependencies
# this ensures rebuilding of binding when
# one of those headers changes.
SET_SOURCE_FILES_PROPERTIES(pyeye.i PROPERTIES
        CPLUSPLUS ON
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/log/EyeLog.h 
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/log/Shapes.h 
        )

# this rule is commented out, because otherwise system headers
# included in our headers get included aswell.
#SET_SOURCE_FILES_PROPERTIES(pyeye.i PROPERTIES SWIG_FLAGS "-includeall")

SWIG_ADD_MODULE(pyeye python pyeye.i) 
SWIG_LINK_LIBRARIES(pyeye ${PYTHON_LIBRARIES} eyelog)
