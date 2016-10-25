
file(GLOB_RECURSE sources
${CMAKE_SOURCE_DIR}/papas/*.cpp
)
file(GLOB_RECURSE headers
${CMAKE_SOURCE_DIR}/papas/*.h
)

project(papaslibrary)

include_directories(
${CMAKE_SOURCE_DIR})

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






