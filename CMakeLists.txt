cmake_minimum_required(VERSION 2.8)
project(fastsim)

### where to find the libraries
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
### where to find the executables
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}") 

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

### add CERN ROOT
find_package(ROOT REQUIRED COMPONENTS Core Hist Gpad MathCore Physics  Geom GRAF GRAF3D  RIO Hist Tree Net   )

include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/fastsim ${CMAKE_SOURCE_DIR}/utility ${CMAKE_SOURCE_DIR}/detectors ${CMAKE_SOURCE_DIR}/display
${CMAKE_SOURCE_DIR}/datatypes ${CMAKE_SOURCE_DIR}/simulation ${CMAKE_SOURCE_DIR}/DAGtool )

add_subdirectory(fastsim)
add_subdirectory(main)
add_subdirectory(DAGtool)
#add_subdirectory(tests)






