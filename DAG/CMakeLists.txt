#Set compiler flags

#mkdir build
#cd build
#cmake -G Xcode ..b

cmake_minimum_required(VERSION 3.3)
project(DAG_examples)

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
include_directories( ${Boost_INCLUDE_DIRS} /Users/alice/fcc/cpp/papas/src/  /Users/alice/fcc/cpp/papas/src/DAG/ /Users/alice/fcc/cpp/papas/src/utility/
/Users/alice/fcc/cpp/papas/src/display/)

add_executable(polytreeany DAG_example_boostany.cc  directedacyclicgraph.h  ../utility/enumhelper.h  ../utility/identifier.h  ../utility/identifier.cc)

add_executable(polytreeid DAG_example_longidnode.cc  directedacyclicgraph.h
../utility/enumhelper.h  ../utility/identifier.h ../utility/identifier.cc)

add_executable(polytreepoly DAG_example_polymorphic.cc  directedacyclicgraph.h
../utility/enumhelper.h  ../utility/identifier.h ../utility/identifier.cc)

#add_executable(polytree  experiment.cpp template_polytree.h)
target_link_libraries(polytreeid )
target_link_libraries(polytreeany )
target_link_libraries(polytreepoly )
