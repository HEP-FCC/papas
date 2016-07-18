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




SET(CMAKE_INSTALL_PATH "${CMAKE_SOURCE_DIR}/install")
Message(STATUS,"install" ${CMAKE_INSTALL_PATH} )

#make this automated
set (MAINDIR ${CMAKE_SOURCE_DIR})
file(GLOB_RECURSE sources     
${MAINDIR}/datatypes/src/*.cpp 
${MAINDIR}/graphtools/src/*.cpp ${MAINDIR}/graphtools/papas/*.h
${MAINDIR}/fastsim/src/*.cpp ${MAINDIR}/fastsim/papas/*.h
${MAINDIR}/utility/src/*.cpp ${MAINDIR}/utility/papas/*.h
${MAINDIR}/display/src/*.cpp ${MAINDIR}/display/papas/*.h
${MAINDIR}/detectors/src/*.cpp ${MAINDIR}/detectors/papas/*.h
${MAINDIR}/simulation/src/*.cpp ${MAINDIR}/simulation/papas/*.h
${MAINDIR}/reconstruction/src/*.cpp ${MAINDIR}/reconstruction/papas/*.h


)

file(GLOB_RECURSE headers 
${MAINDIR}/datatypes/papas/*.h
${MAINDIR}/display/papas/*.h
${MAINDIR}/detectors/papas/*.h
${MAINDIR}/simulation/papas/*.h
${MAINDIR}/graphtools/papas/*.h
${MAINDIR}/utility/papas/*.h
${MAINDIR}/reconstruction/papas/*.h
${MAINDIR}/spdlog
)

include_directories(
${MAINDIR}/datatypes/papas
${MAINDIR}/display/papas
${MAINDIR}/detectors/papas
${MAINDIR}/simulation/papas
${MAINDIR}/graphtools/papas
${MAINDIR}/utility/papas
${MAINDIR}/reconstruction/papas
${MAINDIR}/spdlog
)


project(event)

## You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
##   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmak#e
##   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED )

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#include(${ROOT_USE_FILE}  )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

#find_package(GTest)
#include_directories(${GTEST_INCLUDE_DIRS})





#include_directories(
#  "${ROOT_INCLUDE_DIRS}"
#  )

REFLEX_GENERATE_DICTIONARY(papascppstuffDict ${headers} SELECTION selection.xml)

message(STATUS, "bin" ${PROJECT_BINARY_DIR})


add_library(papascppstuff SHARED ${sources} 
${MAINDIR}/graphtools/papas/directedacyclicgraph.h
${MAINDIR}/utility/src/random.cpp
${MAINDIR}/utility/papas/random.h papascppstuffDict.cxx)
target_link_libraries(papascppstuff ${ROOT_LIBRARIES}  ${ROOT_COMPONENT_LIBRARIES}) 

install(TARGETS papascppstuff DESTINATION lib)

### Install the dictionary
if (${ROOT_VERSION} GREATER 6)
install(FILES
"${PROJECT_BINARY_DIR}/papascppstuffDict_rdict.pcm"
DESTINATION lib COMPONENT dev)
endif()







