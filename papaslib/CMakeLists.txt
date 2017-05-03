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
         ${CMAKE_SOURCE_DIR}/spdlog/*.h
         ${CMAKE_SOURCE_DIR}/spdlog/sinks/*.h
         ${CMAKE_SOURCE_DIR}/spdlog/details/*.h
         )

#message("Headers: ${headers}")
#message("Sources: ${sources}")

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
FILE(GLOB files8 "${CMAKE_SOURCE_DIR}/spdlog/*.*")
INSTALL(FILES ${files8} DESTINATION include/spdlog/)
FILE(GLOB files9 "${CMAKE_SOURCE_DIR}/spdlog/details/*.h")
INSTALL(FILES ${files9} DESTINATION include/spdlog/details/)
FILE(GLOB files11 "${CMAKE_SOURCE_DIR}/spdlog/details/*.cc")
INSTALL(FILES ${files11} DESTINATION include/spdlog/details/)
FILE(GLOB files10 "${CMAKE_SOURCE_DIR}/spdlog/sinks/*.*")
INSTALL(FILES ${files10} DESTINATION include/spdlog/sinks/)


project(papaslib)

include_directories(
${CMAKE_SOURCE_DIR}
)

# remove ### to get back root dicts
###REFLEX_GENERATE_DICTIONARY(papas ${headers} SELECTION selection.xml)

#message(STATUS, "bin" ${PROJECT_BINARY_DIR})

###add_library(papasDict SHARED ${sources} ${headers} papas.cxx)
###add_dependencies(papasDict papas-dictgen )
###target_link_libraries( papasDict papas ${ROOT_LIBRARIES} )
add_library(papas SHARED ${sources} )
target_link_libraries(  papas ${ROOT_LIBRARIES} )
target_compile_definitions(papas PRIVATE WITHSORT=1)
install(TARGETS papas DESTINATION lib)
###install(TARGETS papasDict DESTINATION lib)

# Install the dictionary
###install(FILES ${CMAKE_CURRENT_BINARY_DIR}/papasDict.rootmap DESTINATION lib)
###install(FILES ${CMAKE_CURRENT_BINARY_DIR}/papas_rdict.pcm DESTINATION lib)

#install(DIRECTORY sources DESTINATION include)

### Install the dictionary
#if (${ROOT_VERSION} GREATER 6)
#install(FILES
#"${PROJECT_BINARY_DIR}/papas_rdict.pcm"
#DESTINATION lib COMPONENT dev)
#endif()









