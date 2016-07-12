

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

#include_directories(
#"${podio_INCLUDE_DIRS}"
#"${FCCEDM_INCLUDE_DIRS}"
#)

include_directories(/usr/local/lib ${CMAKE_SOURCE_DIR}/install/lib)

message(status "${podio_INCLUDE_DIRS}")
message(status "ffccedm"  "${FCCEDM_INCLUDE_DIRS}")
message(status $ENV{ROOT_INCLUDE_PATH})
message(status $ENV{DYLD_LIBRARY_PATH})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#include(${ROOT_USE_FILE})

#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

#find_package(GTest)
#include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include  )
message(status, "Alice main")
add_executable(mainexe ../main.cpp pythiaconnector.cpp)
target_link_libraries(mainexe papascpp ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities) #${GTEST_LIBRARIES}
#target_link_libraries(mainexe papascppstuff ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities) #${GTEST_LIBRARIES}






