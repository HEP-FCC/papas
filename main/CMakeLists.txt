cmake_minimum_required(VERSION 2.8)
#Set compiler flags


#Set compiler flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.59.0 REQUIRED)
message("Boost searched for")
if (Boost_FOUND)
message("Boost Found")
include_directories(
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/graphtools)
endif()
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
)

include_directories(/usr/local/lib ${CMAKE_SOURCE_DIR}/install/lib)


#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include /usr/local/lib )

add_executable(mainexe ../main.cpp)
target_link_libraries(mainexe papascpp ${ROOT_LIBRARIES} ${GTEST_LIBRARIES} )
#/users/alice/fcc/cpp/papas/install/lib/libpapascpp.dylib)





