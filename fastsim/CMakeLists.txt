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

#this should come from env variables..... but isn't working just yet'
set(BOOST_ROOT /usr/local/boost_1_59_0)
find_package(Boost 1.59.0 REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS}   /Users/alice/fcc/cpp/papas/src/  )


ROOT_GENERATE_DICTIONARY(fastsim_Dict ../fastsim/my_utilities.h  LINKDEF ../fastsim/my_utilities_LinkDef.h)
add_library(fastsim SHARED
../detectors/CMS.h
../detectors/detector.h
../detectors/geometry.h
../detectors/material.h
../pfobjects/particle.h
../pfobjects/pfobjects.h
../pfobjects/baseparticle.h
../simulation/simulator.h
../utility/identifier.h
../utility/identifier.cc
../DAG/directedacyclicgraph.h
../fastsim/my_utilities.h
../fastsim/my_utilities.cpp
../detectors/material.cc
../detectors/detector.cc
../detectors/geometry.cc
../detectors/CMS.cc
../pfobjects/particle.cc
../pfobjects/baseparticle.cc
../pfobjects/pfobjects.cc
../simulation/simulator.cc
../utility/enummanager.h
../utility/enumhelper.h
../pfobjects/path.h
../pfobjects/path.cc
../utility/deltar.h
../utility/deltar.cc
../display/displaygeometry.cc
../display/displaygeometry.h
../display/displaycore.cc
../display/displaycore.h
../display/displaypfobjects.cc
../display/displaypfobjects.h
fastsim_Dict.cxx)
target_link_libraries(fastsim ${ROOT_LIBRARIES}  )

FIND_PACKAGE( Doxygen )
IF( DOXYGEN_FOUND )

# Add doc target
ADD_CUSTOM_TARGET( doc )
ADD_CUSTOM_COMMAND( TARGET doc
COMMAND "${CMAKE_COMMAND}" -E copy_if_different
"${PROJECT_SOURCE_DIR}/README.md" "${PROJECT_SOURCE_DIR}/doc/README.md"
COMMAND "${DOXYGEN_EXECUTABLE}" ARGS "Doxyfile"
WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
COMMENT "Building API Documentation in ${PROJECT_SOURCE_DIR}/doc... "
VERBATIM )

ELSE()
MESSAGE( STATUS "Doxygen not found in your system!!" )
IF( INSTALL_DOC )
MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
SET( INSTALL_DOC OFF )
ENDIF()
ENDIF()







