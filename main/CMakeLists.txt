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

#this should come from env variables..... but isn't working just yet'
set(BOOST_ROOT /usr/local/boost_1_59_0)
find_package(Boost 1.59.0 REQUIRED)
set (PAPAS_ROOT /Users/alice/fcc/cpp/papas/src/)
include_directories( ${Boost_INCLUDE_DIRS})


add_executable(main ../main.cpp)
target_link_libraries(main fastsim ${ROOT_LIBRARIES} )



