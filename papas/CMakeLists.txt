


#make this automated
set (MAINDIR ${CMAKE_SOURCE_DIR}/papas)
file(GLOB_RECURSE sources *.cpp)

#${MAINDIR}/graphtools/src/*.cpp
#${MAINDIR}/utility/src/*.cpp
#${MAINDIR}/display/src/*.cpp
#${MAINDIR}/detectors/src/*.cpp
#${MAINDIR}/simulation/*.cpp
#${MAINDIR}/reconstruction/src/*.cpp
#)

file(GLOB_RECURSE headers *.h)
#${MAINDIR}/datatypes/*.h
#${MAINDIR}/display/*.h
#${MAINDIR}/detectors/*.h
#${MAINDIR}/simulation/*.h
#${MAINDIR}/graphtools/*.h
#${MAINDIR}/utility/*.h
#${MAINDIR}/reconstruction/*.h
#)

project(papaslibrary)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR})

message(status, "building papascpp library")

REFLEX_GENERATE_DICTIONARY(papascppDict ${headers} SELECTION selection.xml)
add_library(papascpp SHARED ${sources}  ${headers}  papascppDict.cxx )
add_dependencies(papascpp papascppDict-dictgen )

target_link_libraries(papascpp ${ROOT_LIBRARIES}  ${ROOT_COMPONENT_LIBRARIES} )
target_compile_definitions(papascpp PRIVATE WITHSORT=1)
install(TARGETS papascpp DESTINATION lib)

# Install the dictionary
if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/papascppDict_rdict.pcm"
      DESTINATION lib COMPONENT dev)
endif()






