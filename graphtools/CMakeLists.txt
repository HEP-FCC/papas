#Set compiler flags

#mkdir build
#cd build
#cmake -G Xcode ..
#cmake ..

cmake_minimum_required(VERSION 3.3)
project(DAG_examples)



include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/examples
        ${CMAKE_CURRENT_SOURCE_DIR}/graphtools
)

file(GLOB headers examples/*.h directedacyclicgraph.h)
#add_executable(DAG_id examples/DAG_example_longidnode.cpp   examples/simpleidentifier.cpp )
#add_executable(DAG_poly examples/DAG_example_polymorphic.cpp  graphtools/directedacyclicgraph.h )
#add_executable(DAG_id2 examples/DAG_example_longidnode.cpp   examples/simpleidentifier.cpp )

#target_link_libraries(DAG_id )
#target_link_libraries(DAG_id2 )
#target_link_libraries(DAG_poly )

#If the Boost Package is found then the fianl example can be compiled.
#this should come from env variables
#set(BOOST_ROOT /usr/local/boost_1_59_0)
find_package(Boost)
if (Boost_FOUND)
include_directories( ${Boost_INCLUDE_DIRS} )
#add_executable(DAG_any examples/DAG_example_boostany.cpp  ${headers} )
#target_link_libraries(DAG_any )
endif()