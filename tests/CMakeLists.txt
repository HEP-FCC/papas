message(status, "building unittests")

set (MAINDIR "${CMAKE_SOURCE_DIR}")

include_directories(
${MAINDIR}/utility/papas
${MAINDIR}/detectors/papas
${MAINDIR}/display/papas
${MAINDIR}/datatypes/papas/
${MAINDIR}/simulation/papas
${MAINDIR}/reconstruction/papas
${MAINDIR}/graphtools/papas
${CMAKE_CURRENT_SOURCE_DIR}
)



add_executable(utests unittest.cpp )
target_link_libraries(utests  papascpp ${ROOT_LIBRARIES} )


install(TARGETS utests DESTINATION bin)


# --- adding tests for examples ------------------------------
add_test(NAME utests COMMAND utests)







