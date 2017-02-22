message(status, "building unittests")

set (MAINDIR "${CMAKE_SOURCE_DIR}")

include_directories(
${MAINDIR}
${MAINDIR}/papas/tests
${CMAKE_CURRENT_SOURCE_DIR}
)

message(${MAINDIR} ${INCLUDE_DIRECTORIES})

add_executable(utests unittest.cpp )
target_link_libraries(utests  papas ${ROOT_LIBRARIES} datamodel datamodelDict podio utilities)


install(TARGETS utests DESTINATION bin)


# --- adding tests for examples ------------------------------
add_test(NAME utests COMMAND utests)







