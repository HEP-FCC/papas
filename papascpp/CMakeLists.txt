cmake_minimum_required(VERSION 2.8)

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


#Set compiler flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()



# Make sure we find the Find*.cmake functions distributed with this package
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{PODIO} $ENV{FCCEDM})

find_package(podio REQUIRED HINTS $ENV{PODIO})
message(STATUS "Found podio: ${podio_DIR}")
# Make sure the library is found.
# Not the case if LD_LIBRARY_PATH is wrong
find_library(PODIOLIB podio)
if (NOT PODIOLIB)
message(FATAL_ERROR "libpodio.so(dylib) cannot be found dynamically. Make sure you have sourced PODIO init*.sh file to set up your environment to use PODIO")
endif()
link_directories(${podio_LIBRARY_DIR})

find_package(fccedm REQUIRED HINTS $ENV{FCCEDM})
message(STATUS "Found fcc-edm: ${fccedm_DIR}")



MESSAGE( STATUS "Doxygen engaging " )
FIND_PACKAGE( Doxygen )
IF( DOXYGEN_FOUND )

# Add doc target
ADD_CUSTOM_TARGET( doc )
ADD_CUSTOM_COMMAND( TARGET doc
COMMAND "${CMAKE_COMMAND}" -E copy_if_different
"${PROJECT_SOURCE_DIR}/../README.md" "${PROJECT_SOURCE_DIR}/../doc/README.md"
COMMAND "${DOXYGEN_EXECUTABLE}" ARGS "Doxyfile"
WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../doc"
COMMENT "Building API Documentation in ${PROJECT_SOURCE_DIR}/../doc... "
VERBATIM )

ELSE()
MESSAGE( STATUS "Doxygen not found in your system!!" )
IF( INSTALL_DOC )
MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
SET( INSTALL_DOC OFF )
ENDIF()
ENDIF()

SET(CMAKE_INSTALL_PATH "${CMAKE_SOURCE_DIR}/../install")
Message(STATUS,"install" ${CMAKE_INSTALL_PATH} )

#make this automated
set (MAINDIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE sources     
${MAINDIR}/datatypes/src/*.cpp ${MAINDIR}/datatypes/datatypes/*.h
${MAINDIR}/graphtools/src/*.cpp ${MAINDIR}/graphtools/graphtools/*.h
${MAINDIR}/fastsim/src/*.cpp ${MAINDIR}/fastsim/fastsim/*.h
${MAINDIR}/utility/src/*.cpp ${MAINDIR}/utility/utility/*.h
${MAINDIR}/display/src/*.cpp ${MAINDIR}/display/display/*.h
${MAINDIR}/detectors/src/*.cpp ${MAINDIR}/detectors/detectors/*.h
${MAINDIR}/simulation/src/*.cpp ${MAINDIR}/simulation/simulation/*.h
${MAINDIR}/reconstruction/src/*.cpp ${MAINDIR}/reconstruction/reconstruction/*.h


)


file(GLOB_RECURSE headers 
${MAINDIR}/datatypes/datatypes/*.h
${MAINDIR}/display/display/*.h
${MAINDIR}/detectors/detectors/*.h
${MAINDIR}/simulation/simulation/*.h
${MAINDIR}/graphtools/graphtools/*.h
${MAINDIR}/utility/utility/*.h
${MAINDIR}/reconstruction/reconstruction/*.h
)

project(event)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED )

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE}  )

include_directories(

"${podio_INCLUDE_DIRS}"
"${FCCEDM_INCLUDE_DIRS}"
)
message(status, "${podio_INCLUDE_DIRS}")
message(status, "${FCCEDM_INCLUDE_DIRS}")
message(status $ENV{ROOT_INCLUDE_PATH})
message(status $ENV{DYLD_LIBRARY_PATH})

include_directories(

"${ROOT_INCLUDE_DIRS}"
)
include_directories(
${MAINDIR}/datatypes/datatypes/
${MAINDIR}/display/display/
${MAINDIR}/detectors/detectors/
${MAINDIR}/simulation/simulation/
${MAINDIR}/graphtools/graphtools/
${MAINDIR}/utility/utility/
${MAINDIR}/reconstruction/reconstruction/
${MAINDIR}/spdlog )
#${CMAKE_SOURCE_DIR} )

add_definitions(${ROOT_CXX_FLAGS})

#find_package(GTest)
#include_directories(${GTEST_INCLUDE_DIRS})




REFLEX_GENERATE_DICTIONARY(papascppDict ${headers} SELECTION selection.xml)

message(STATUS, "bin" ${PROJECT_BINARY_DIR})

add_library(papascpp SHARED ${sources} 
${MAINDIR}/graphtools/graphtools/directedacyclicgraph.h
${MAINDIR}/utility/src/random.cpp
${MAINDIR}/utility/utility/random.h ) # papascppDict.cxx)
target_link_libraries(papascpp ${ROOT_LIBRARIES}  ${ROOT_COMPONENT_LIBRARIES} )








