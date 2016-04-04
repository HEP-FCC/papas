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
#CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#if(COMPILER_SUPPORTS_CXX14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#else
if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

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

#this should come from env variables
#set(BOOST_ROOT /usr/local/boost_1_59_0)
#set (DYLD_LIBRARY_PATH /Users/alice/local/lib/root)
find_package(Boost 1.59.0 REQUIRED)
if (BOOST_found)
include_directories( ${Boost_INCLUDE_DIRS})
endif()
#make this automated
set (MAINDIR /Users/alice/fcc/cpp/papas/src/)
file(GLOB_RECURSE sources     
${MAINDIR}/datatypes/*.cc ${MAINDIR}/datatypes/*.h
${MAINDIR}/fastsim/*.cc ${MAINDIR}/fastsim/*.h
${MAINDIR}/utility/*.cc ${MAINDIR}/utility/*.h
${MAINDIR}/display/*.cc ${MAINDIR}/display/*.h
${MAINDIR}/detectors/*.cc ${MAINDIR}/detectors/*.h
${MAINDIR}/simulation/*.cc ${MAINDIR}/simulation/*.h
${MAINDIR}/pfalgo/*.cc ${MAINDIR}/pfalgo/*.h)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${MAINDIR}/datatypes/
${MAINDIR}/fastsim/
${MAINDIR}/display/
${MAINDIR}/detectors/
${MAINDIR}/simulation/
${MAINDIR}/utility
${MAINDIR}/utility/
${MAINDIR}/DAGtool/
${MAINDIR}/pfalgo/
)

project(event)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE}  )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})


find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})




#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

ROOT_GENERATE_DICTIONARY(fastsim_Dict my_utilities.h  LINKDEF my_utilities_LinkDef.h)
add_library(fastsim SHARED ${sources} ${MAINDIR}/DAGtool/directedacyclicgraph.h
${MAINDIR}/utility/random.cc ${MAINDIR}/utility/random.h fastsim_Dict.cxx)
target_link_libraries(fastsim ${ROOT_LIBRARIES}  ${GTEST_LIBRARIES} )









