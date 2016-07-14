


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

project(papaslibrary)

include_directories(
${MAINDIR}/datatypes/datatypes/
${MAINDIR}/display/display/
${MAINDIR}/detectors/detectors/
${MAINDIR}/simulation/simulation/
${MAINDIR}/graphtools/graphtools/
${MAINDIR}/utility/utility/
${MAINDIR}/reconstruction/reconstruction/
${MAINDIR}/spdlog )

message(status, "building papascpp library")

REFLEX_GENERATE_DICTIONARY(papascppDict ${headers} SELECTION selection.xml)
add_library(papascpp SHARED ${sources}  ${headers} papascppDict )
add_dependencies(papascpp papascppDict-dictgen )

target_link_libraries(papascpp ${ROOT_LIBRARIES}  ${ROOT_COMPONENT_LIBRARIES} )

install(TARGETS papascpp DESTINATION lib)

# Install the dictionary
if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/papascppDict_rdict.pcm"
      DESTINATION lib COMPONENT dev)
endif()






