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

#this should come from env variables
#set(BOOST_ROOT /usr/local/boost_1_59_0)
find_package(Boost 1.59.0 REQUIRED)
set (PAPAS_ROOT /Users/alice/fcc/cpp/papas/src/)
include_directories( ${Boost_INCLUDE_DIRS} )

include_directories(${CMAKE_SOURCE_DIR}/fastsim ${CMAKE_SOURCE_DIR}/utility ${CMAKE_SOURCE_DIR}/detectors ${CMAKE_SOURCE_DIR}/display
${CMAKE_SOURCE_DIR}/datatypes ${CMAKE_SOURCE_DIR}/simulation ${CMAKE_SOURCE_DIR}/DAGtool )

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

#set (DYLD_LIBRARY_PATH /Users/alice/local/root/lib)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include /usr/local/lib )
message(${ROOT_LIBRARIES})

add_executable(tests tests.cpp)
target_link_libraries(tests fastsim ${ROOT_LIBRARIES} ${GTEST_LIBRARIES})


