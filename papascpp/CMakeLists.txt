




#FIND_PACKAGE( Doxygen HINTS env{$DOXYGEN})
#IF( DOXYGEN_FOUND )
#MESSAGE( STATUS "Doxygen engaging " )
# Add doc target
#ADD_CUSTOM_TARGET( doc )
#ADD_CUSTOM_COMMAND( TARGET doc
#COMMAND "${CMAKE_COMMAND}" -E copy_if_different
#"${PROJECT_SOURCE_DIR}/../README.md" "${PROJECT_SOURCE_DIR}/../doc/README.md"
#COMMAND "${DOXYGEN_EXECUTABLE}" ARGS "Doxyfile"
#WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../doc"
#COMMENT "Building API Documentation in ${PROJECT_SOURCE_DIR}/../doc... "
#VERBATIM )

#ELSE()
#MESSAGE( STATUS "Doxygen not found in your system!!" )
#IF( INSTALL_DOC )
#MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
#SET( INSTALL_DOC OFF )
#ENDIF()
#ENDIF()


SET(CMAKE_INSTALL_PATH "${CMAKE_SOURCE_DIR}/install")
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


message(status, "${podio_INCLUDE_DIRS}")
message(status, "${FCCEDM_INCLUDE_DIRS}")
message(status $ENV{DYLD_LIBRARY_PATH})

include_directories(
${MAINDIR}/datatypes/datatypes/
${MAINDIR}/display/display/
${MAINDIR}/detectors/detectors/
${MAINDIR}/simulation/simulation/
${MAINDIR}/graphtools/graphtools/
${MAINDIR}/utility/utility/
${MAINDIR}/reconstruction/reconstruction/
${MAINDIR}/spdlog )


#find_package(GTest)
#include_directories(${GTEST_INCLUDE_DIRS})




REFLEX_GENERATE_DICTIONARY(papascppDict ${headers} SELECTION selection.xml)

#message(STATUS, "bin" ${PROJECT_BINARY_DIR})

add_library(papascpp SHARED ${sources} )
#${MAINDIR}/graphtools/graphtools/directedacyclicgraph.h
#${MAINDIR}/utility/src/random.cpp
#${MAINDIR}/utility/utility/random.h ) 
target_link_libraries(papascpp ${ROOT_LIBRARIES}  )





