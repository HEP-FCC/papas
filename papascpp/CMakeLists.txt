


#make this automated
set (MAINDIR ${CMAKE_SOURCE_DIR})
file(GLOB_RECURSE sources     
${MAINDIR}/datatypes/src/*.cpp ${MAINDIR}/datatypes/papas/*.h
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
)

project(papaslibrary)

include_directories(
${MAINDIR}/datatypes/papas/
${MAINDIR}/display/papas/
${MAINDIR}/detectors/papas/
${MAINDIR}/simulation/papas/
${MAINDIR}/graphtools/papas/
${MAINDIR}/utility/papas/
${MAINDIR}/reconstruction/papas/
${MAINDIR}/spdlog)

message(status, "building papascpp library")

REFLEX_GENERATE_DICTIONARY(papascppDict ${headers} SELECTION selection.xml)
add_library(papascpp SHARED ${sources}  ${headers} papascppDict.cxx )
add_dependencies(papascpp papascppDict-dictgen )

target_link_libraries(papascpp ${ROOT_LIBRARIES}  ${ROOT_COMPONENT_LIBRARIES} )

install(TARGETS papascpp DESTINATION lib)

# Install the dictionary
if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/papascppDict_rdict.pcm"
      DESTINATION lib COMPONENT dev)
endif()






