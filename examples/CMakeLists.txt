set (MAINDIR "${CMAKE_SOURCE_DIR}")

include_directories(
${MAINDIR}/utility/papas
${MAINDIR}/detectors/papas
${MAINDIR}/display/papas
${MAINDIR}/datatypes/papas/
${MAINDIR}/simulation/papas
${MAINDIR}/reconstruction/papas
${MAINDIR}/graphtools/papas
${MAINDIR}/spdlog
${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(example_simple example_simple.cpp PythiaConnector.cpp)
target_link_libraries(example_simple papascpp ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities)

add_executable(example_loop example_loop.cpp PythiaConnector.cpp)
target_link_libraries(example_loop papascpp ${ROOT_LIBRARIES}  datamodel datamodelDict podio utilities)

#${podio_LIBRARY_DIR}/libpodio.so  ${podio_LIBRARY_DIR}/libpodioDict.so 
#/Users/alice/fccwork/fcc-edm/install/lib/libdatamodel.so  /Users/alice/fccwork/fcc-edm/install/lib/libdatamodelDict.so )


install(TARGETS example_simple DESTINATION bin)
install(TARGETS example_loop DESTINATION bin)

# --- adding tests for examples ------------------------------
add_test(NAME fcc-generate COMMAND $ENV{FCCPHYSICS}/bin/fcc-pythia8-generate $ENV{FCCPHYSICS}/share/ee_ZH_Zmumu_Hbb.txt WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_test(NAME example_loop  COMMAND $ENV{FCCPAPASCPP}/bin/example_loop ee_ZH_Zmumu_Hbb.root  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  )
#set_property(TEST example_loop PROPERTY DEPENDS fcc-generate)