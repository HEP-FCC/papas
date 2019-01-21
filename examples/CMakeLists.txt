set (MAINDIR "${CMAKE_SOURCE_DIR}")

include_directories(
${MAINDIR}
${MAINDIR}/spdlog
${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(example_simple example_simple.cpp PythiaConnector.cpp )
target_link_libraries(example_simple papas datamodel datamodelDict podio utilities)

add_executable(example_loop example_loop.cpp PythiaConnector.cpp)
target_link_libraries(example_loop papas datamodel datamodelDict podio utilities)

add_executable(example_pair example_dag_pair.cpp)
target_link_libraries(example_pair papas)

add_executable(example_pdebug example_pdebug_python_comparer.cpp  PythiaConnector.cpp )
target_link_libraries(example_pdebug papas datamodel datamodelDict podio utilities)

add_executable(example_plot example_plot.cpp  PythiaConnector.cpp )
target_link_libraries(example_plot papas datamodel datamodelDict podio utilities)

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

file(COPY "${CMAKE_SOURCE_DIR}/examples/data" DESTINATION "${CMAKE_BINARY_DIR}" )

# --- adding tests for examples ------------------------------
add_test(NAME example_loop  COMMAND bin/example_loop data/ee_ZH_Zmumu_Hbb.root WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST example_loop PROPERTY DEPENDS fcc-generate)
add_test(NAME test_debug_cms COMMAND /bin/bash data/pdebug_CMS.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(test_debug_cms PROPERTIES PASS_REGULAR_EXPRESSION "compare.out matches")
add_test(NAME test_debug_clic COMMAND /bin/bash data/pdebug_CLIC.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(test_debug_clic PROPERTIES PASS_REGULAR_EXPRESSION "compare.out matches")
