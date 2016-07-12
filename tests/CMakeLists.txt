

#find_package(Boost 1.59.0 REQUIRED)
#if (BOOST_found)
#include_directories( ${Boost_INCLUDE_DIRS})
#endif()
#FIND_PACKAGE(Boost 1.59.0)
#SET(Boost_USE_STATIC_LIBS OFF)
#SET(Boost_USE_MULTITHREADED ON)
#SET(Boost_USE_STATIC_RUNTIME OFF)
#Message(STATUS  "here1")
#FIND_PACKAGE(Boost 1.59.0 COMPONENTS python)


set (MAINDIR "${CMAKE_SOURCE_DIR}/papascpp")
include_directories(
${MAINDIR}/utility/utility
${MAINDIR}/detectors/detectors
${MAINDIR}/display/display
${MAINDIR}/datatypes/datatypes
${MAINDIR}/simulation/simulation
${MAINDIR}/graphtools/graphtools )


add_executable(tests unittest.cpp )
target_link_libraries(tests  papascppstuff ${ROOT_LIBRARIES} ) 






