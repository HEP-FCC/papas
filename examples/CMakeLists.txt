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
target_compile_definitions(example_loop PRIVATE WITHSORT=1)
target_link_libraries(example_loop papascpp ${ROOT_LIBRARIES}  datamodel datamodelDict podio utilities)

add_executable(example_pair example_dag_pair.cpp)
target_link_libraries(example_pair papascpp ${ROOT_LIBRARIES} )

add_executable(example_pdebug example_pdebug_python_comparer.cpp  PythiaConnector.cpp )

target_link_libraries(example_pdebug papascpp ${ROOT_LIBRARIES} datamodel datamodelDict podio utilities)

add_executable(example_plot example_plot.cpp  PythiaConnector.cpp )
target_link_libraries(example_plot papascpp ${ROOT_LIBRARIES} datamodel datamodelDict podio utilities)

add_executable(example_gun example_gun.cpp )
target_link_libraries(example_gun papascpp ${ROOT_LIBRARIES} datamodel datamodelDict utilities)

#add_executable(example_root example_root.cpp example_frame.cpp)
#target_link_libraries(example_root  papascpp ${ROOT_LIBRARIES} datamodelDict )

#${podio_LIBRARY_DIR}/libpodio.so  ${podio_LIBRARY_DIR}/libpodioDict.so 
#/Users/alice/fccwork/fcc-edm/install/lib/libdatamodel.so  /Users/alice/fccwork/fcc-edm/install/lib/libdatamodelDict.so )


install(TARGETS example_simple DESTINATION bin)
install(TARGETS example_loop DESTINATION bin)
install(TARGETS example_pdebug DESTINATION bin)
install(TARGETS example_plot DESTINATION bin)
#install(TARGETS example_root DESTINATION bin)

# --- adding tests for examples ------------------------------
add_test(NAME fcc-generate COMMAND $ENV{FCCPHYSICS}/bin/fcc-pythia8-generate $ENV{FCCPHYSICS}/share/ee_ZH_Zmumu_Hbb.txt WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_test(NAME example_loop  COMMAND $ENV{FCCPAPASCPP}/bin/example_loop ee_ZH_Zmumu_Hbb.root  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  )

#set_property(TEST example_loop PROPERTY DEPENDS fcc-generate)