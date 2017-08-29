set (MAINDIR "${CMAKE_SOURCE_DIR}")

include_directories(
${MAINDIR}
${MAINDIR}/spdlog
${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(example_simple example_simple.cpp PythiaConnector.cpp )
target_compile_definitions(example_simple PRIVATE WITHSORT=1)
target_link_libraries(example_simple papas ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities)

add_executable(example_loop example_loop.cpp PythiaConnector.cpp)
target_compile_definitions(example_loop PRIVATE WITHSORT=1)
target_link_libraries(example_loop papas ${ROOT_LIBRARIES}  datamodel datamodelDict podio utilities)

add_executable(example_pair example_dag_pair.cpp)
target_link_libraries(example_pair papas ${ROOT_LIBRARIES} )

add_executable(example_pdebug example_pdebug_python_comparer.cpp  PythiaConnector.cpp )
target_compile_definitions(example_pdebug PRIVATE WITHSORT=1)
target_link_libraries(example_pdebug papas ${ROOT_LIBRARIES} datamodel datamodelDict podio utilities)

add_executable(example_plot example_plot.cpp  PythiaConnector.cpp )
target_link_libraries(example_plot papas ${ROOT_LIBRARIES} datamodel datamodelDict podio utilities)

#todo fix this
#add_executable(example_gun example_gun.cpp )
#target_link_libraries(example_gun papas ${ROOT_LIBRARIES} datamodel datamodelDict utilities)

#add_executable(example_root example_root.cpp example_frame.cpp)
#target_link_libraries(example_root  papas ${ROOT_LIBRARIES} datamodelDict )

#${podio_LIBRARY_DIR}/libpodio.so  ${podio_LIBRARY_DIR}/libpodioDict.so 
#/Users/alice/fccwork/fcc-edm/install/lib/libdatamodel.so  /Users/alice/fccwork/fcc-edm/install/lib/libdatamodelDict.so )


install(TARGETS example_simple DESTINATION bin)
install(TARGETS example_loop DESTINATION bin)
install(TARGETS example_pdebug DESTINATION bin)
install(TARGETS example_plot DESTINATION bin)

FILE(GLOB files "${CMAKE_SOURCE_DIR}/examples/data/*")
INSTALL(FILES ${files} DESTINATION data/)

# --- adding tests for examples ------------------------------
add_test(NAME fcc-generate COMMAND $ENV{FCCPHYSICS}/bin/fcc-pythia8-generate $ENV{FCCPHYSICS}/share/ee_ZH_Zmumu_Hbb.txt WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_test(NAME example_loop  COMMAND $ENV{FCCPAPASCPP}/bin/example_loop ee_ZH_Zmumu_Hbb.root  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  )
set_property(TEST example_loop PROPERTY DEPENDS fcc-generate)
add_test(NAME test_debug_cms COMMAND /bin/bash $ENV{FCCPAPASCPP}/data/pdebug_CMS.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  )
set_tests_properties(test_debug_cms PROPERTIES PASS_REGULAR_EXPRESSION "compare.out matches")
set_property(TEST test_debug_cms PROPERTY DEPENDS fcc-generate)
add_test(NAME test_debug_clic COMMAND /bin/bash $ENV{FCCPAPASCPP}/data/pdebug_CLIC.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  )
set_tests_properties(test_debug_clic PROPERTIES PASS_REGULAR_EXPRESSION "compare.out matches")
set_property(TEST test_debug_clic PROPERTY DEPENDS fcc-generate)
