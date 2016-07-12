set (MAINDIR "${CMAKE_SOURCE_DIR}/papascpp")

include_directories(
${MAINDIR}/utility/utility
${MAINDIR}/detectors/detectors
${MAINDIR}/display/display
${MAINDIR}/datatypes/datatypes/
${MAINDIR}/simulation/simulation
${MAINDIR}/reconstruction/reconstruction
${MAINDIR}/graphtools/graphtools
${MAINDIR}/spdlog
${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(example_simple example_simple.cpp pythiaconnector.cpp)
target_link_libraries(example_simple papascppstuff ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities)

add_executable(example_loop example_loop.cpp pythiaconnector.cpp)
target_link_libraries(example_loop papascppstuff ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities)

install(TARGETS example_simple DESTINATION bin)
install(TARGETS example_loop DESTINATION bin)



# --- adding tests for examples ------------------------------
add_test(NAME example_loop COMMAND example_loop /Users/alice/fcc/pythiafiles/ee_ZH_Zmumu_Hbb.root)

