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



include_directories(
${CMAKE_SOURCE_DIR}
)

SET(CMAKE_INSTALL_PATH "${CMAKE_SOURCE_DIR}/install")
Message(STATUS,"install" ${CMAKE_INSTALL_PATH} )


file(GLOB_RECURSE sources
         
         ${CMAKE_SOURCE_DIR}/papas/datatypes/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/detectors/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/graphtools/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/display/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/reconstruction/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/simulation/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/utility/*.cpp
         )

file(GLOB_RECURSE headers
         
         ${CMAKE_SOURCE_DIR}/papas/datatypes/*.h
         ${CMAKE_SOURCE_DIR}/papas/detectors/*.h
         ${CMAKE_SOURCE_DIR}/papas/graphtools/*.h
         ${CMAKE_SOURCE_DIR}/papas/display/*.h
         ${CMAKE_SOURCE_DIR}/papas/reconstruction/*.h
         ${CMAKE_SOURCE_DIR}/papas/simulation/*.h
         ${CMAKE_SOURCE_DIR}/papas/utility/*.h
         )

message("${CMAKE_SOURCE_DIR}")
#message("Headers: ${headers}")
#message("Headers: ${sources}")

FILE(GLOB files "${CMAKE_SOURCE_DIR}/papas/datatypes/*.h")
INSTALL(FILES ${files} DESTINATION include/papas/datatypes/)
FILE(GLOB files2 "${CMAKE_SOURCE_DIR}/papas/simulation/*.h")
INSTALL(FILES ${files2} DESTINATION include/papas/simulation/)
FILE(GLOB files3 "${CMAKE_SOURCE_DIR}/papas/reconstruction/*.h")
INSTALL(FILES ${files3} DESTINATION include/papas/reconstruction/)
FILE(GLOB files4 "${CMAKE_SOURCE_DIR}/papas/detectors/*.h")
INSTALL(FILES ${files4} DESTINATION include/papas/detectors/)
FILE(GLOB files5 "${CMAKE_SOURCE_DIR}/papas/graphtools/*.h")
INSTALL(FILES ${files5} DESTINATION include/papas/graphtools/)
FILE(GLOB files6 "${CMAKE_SOURCE_DIR}/papas/display/*.h")
INSTALL(FILES ${files6} DESTINATION include/papas/display/)
FILE(GLOB files7 "${CMAKE_SOURCE_DIR}/papas/utility/*.h")
INSTALL(FILES ${files7} DESTINATION include/papas/utility/)



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

#include_directories(${CMAKE_SOURCE_DIR}/papas/datatypes/ 
#${CMAKE_SOURCE_DIR}/papas/simulation
#${CMAKE_SOURCE_DIR}/papas/reconstruction
#${CMAKE_SOURCE_DIR}/papas/display
#${CMAKE_SOURCE_DIR}/papas/graphtools
#${CMAKE_SOURCE_DIR}/papas/detectors
#${CMAKE_SOURCE_DIR}/papas/utility
# ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

include_directories(
${CMAKE_SOURCE_DIR}
)

REFLEX_GENERATE_DICTIONARY(papascppstuffDict ${headers} SELECTION selection.xml)

message(STATUS, "bin" ${PROJECT_BINARY_DIR})


add_library(papascppstuff SHARED ${sources} ${headers} papascppstuffDict.cxx)
target_link_libraries(papascppstuff ${ROOT_LIBRARIES}  ${ROOT_COMPONENT_LIBRARIES}) 
install(TARGETS papascppstuff DESTINATION lib)


#install(DIRECTORY sources DESTINATION include)

### Install the dictionary
if (${ROOT_VERSION} GREATER 6)
install(FILES
"${PROJECT_BINARY_DIR}/papascppstuffDict_rdict.pcm"
DESTINATION lib COMPONENT dev)
endif()









