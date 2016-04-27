cmake_minimum_required(VERSION 2.8)
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

find_package(Boost 1.59.0 REQUIRED)
set (PAPAS_ROOT /Users/alice/fcc/cpp/papas/src/)

include_directories( ${Boost_INCLUDE_DIRS} )
include_directories(${CMAKE_SOURCE_DIR}/fastsim
${CMAKE_SOURCE_DIR}/utility/utility
${CMAKE_SOURCE_DIR}/detectors
${CMAKE_SOURCE_DIR}/display
${CMAKE_SOURCE_DIR}/datatypes
${CMAKE_SOURCE_DIR}/simulation
${CMAKE_SOURCE_DIR}/graphtools )

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

FIND_PACKAGE(Boost 1.59.0)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
Message(STATUS  "here1")
FIND_PACKAGE(Boost 1.59.0 COMPONENTS python)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include /usr/local/lib  ${Boost_INCLUDE_DIRS}  "/usr/include/python2.7" ${CMAKE_SOURCE_DIR}/utility/src)


add_executable(othertests tests.cpp ${CMAKE_SOURCE_DIR}/utility/src/rand.cpp)
target_link_libraries(othertests  fastsim ${ROOT_LIBRARIES} ${GTEST_LIBRARIES})






