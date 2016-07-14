message(status, "building main")

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


include_directories(/usr/local/lib ${CMAKE_SOURCE_DIR}/install/lib)

add_definitions(${ROOT_CXX_FLAGS})


add_executable(mainexe ../main.cpp PythiaConnector.cpp)
target_link_libraries(mainexe papascpp ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities) 


install(TARGETS mainexe DESTINATION bin)

#add_test(NAME fcc-gen COMMAND $ENV{FCCPHYSICS}/bin/fcc-pythia8-generate $ENV{FCCPHYSICS}/share/ee_ZH_Zmumu_Hbb.txt WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#add_test(NAME read    COMMAND mainexe ee_ZH_Zmumu_Hbb.root WORKING_DIRECTORY WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#set_property(TEST read PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})
#set_property(TEST read PROPERTY DEPENDS fcc_gen)






