

set (MAINDIR "${CMAKE_SOURCE_DIR}/papascpp")
include_directories(
${MAINDIR}/utility/utility
${MAINDIR}/detectors/detectors
${MAINDIR}/display/display
${MAINDIR}/datatypes/datatypes
${MAINDIR}/simulation/simulation
${MAINDIR}/graphtools/graphtools )


add_executable(utests unittest.cpp )
target_link_libraries(utests  papascpp ${ROOT_LIBRARIES} )


install(TARGETS utests DESTINATION bin)


# --- adding tests for examples ------------------------------
add_test(NAME utests COMMAND utests)







