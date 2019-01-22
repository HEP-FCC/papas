
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

REFLEX_GENERATE_DICTIONARY(papas ${headers} SELECTION selection.xml)
add_library(papasDict SHARED  ${headers}  papas.cxx )
add_dependencies(papasDict papas-dictgen )
target_link_libraries( papasDict papas )
add_library(papas SHARED ${sources} )
#target_link_libraries(  papas ${ROOT_LIBRARIES} )
#target_compile_definitions(papas PRIVATE WITHSORT=1)
install(TARGETS papas DESTINATION lib)

# Install the dictionary
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/papas DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/papasDict.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/papas_rdict.pcm DESTINATION lib)





