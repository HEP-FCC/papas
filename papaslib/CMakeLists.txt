cmake_minimum_required(VERSION 2.8)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  TRUE)

# when building, don't use the install RPATH 
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

include_directories(
${CMAKE_SOURCE_DIR}
)

SET(CMAKE_INSTALL_PATH "${CMAKE_SOURCE_DIR}/install")
Message(STATUS,"install" ${CMAKE_INSTALL_PATH} )

file(GLOB_RECURSE sources
         
         ${CMAKE_SOURCE_DIR}/papas/datatypes/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/detectors/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/detectors/cms/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/detectors/clic/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/graphtools/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/display/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/reconstruction/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/simulation/*.cpp
         ${CMAKE_SOURCE_DIR}/papas/utility/*.cpp
         )

file(GLOB_RECURSE headers
         
         ${CMAKE_SOURCE_DIR}/papas/datatypes/*.h
         ${CMAKE_SOURCE_DIR}/papas/detectors/*.h
         ${CMAKE_SOURCE_DIR}/papas/detectors/cms/*.h
         ${CMAKE_SOURCE_DIR}/papas/detectors/clic/*.h
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
FILE(GLOB files4a "${CMAKE_SOURCE_DIR}/papas/detectors/cms/*.h")
INSTALL(FILES ${files4a} DESTINATION include/papas/detectors/cms/)
FILE(GLOB files4b "${CMAKE_SOURCE_DIR}/papas/detectors/clic/*.h")
INSTALL(FILES ${files4b} DESTINATION include/papas/detectors/clic/)
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

add_library(papas SHARED ${sources} )
target_link_libraries(  papas ${ROOT_LIBRARIES} )
install(TARGETS papas DESTINATION lib)

