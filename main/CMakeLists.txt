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



# Make sure we find the Find*.cmake functions distributed with this package
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{PODIO} $ENV{FCCEDM})

find_package(podio REQUIRED HINTS $ENV{PODIO})
message(STATUS "Found podio: ${podio_DIR}")
# Make sure the library is found.
# Not the case if LD_LIBRARY_PATH is wrong
find_library(PODIOLIB podio)
if (NOT PODIOLIB)
message(FATAL_ERROR "libpodio.so(dylib) cannot be found dynamically. Make sure you have sourced PODIO init*.sh file to set up your environment to use PODIO")
endif()
link_directories(${podio_LIBRARY_DIR})
Message(status,"podio lib",  "${podio_LIBRARY_DIR}")

find_package(fccedm REQUIRED HINTS $ENV{FCCEDM})
message(STATUS "Found fcc-edm: ${fccedm_DIR}")



set (MAINDIR "${CMAKE_SOURCE_DIR}/papascpp")
message("maindir" "${MAINDIR}")
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

include_directories(
"${podio_INCLUDE_DIRS}"
"${FCCEDM_INCLUDE_DIRS}"
)

include_directories(/usr/local/lib ${CMAKE_SOURCE_DIR}/install/lib)

message(status "${podio_INCLUDE_DIRS}")
message(status "ffccedm"  "${FCCEDM_INCLUDE_DIRS}")
message(status $ENV{ROOT_INCLUDE_PATH})
message(status $ENV{DYLD_LIBRARY_PATH})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

#find_package(GTest)
#include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include  )

add_executable(mainexe ../main.cpp)
#target_link_libraries(mainexe papascpp ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities) #${GTEST_LIBRARIES}
target_link_libraries(mainexe papascppstuff ${ROOT_LIBRARIES}   datamodel datamodelDict podio utilities) #${GTEST_LIBRARIES}






