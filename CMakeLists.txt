cmake_minimum_required(VERSION 2.8)

project(papas)
set( papas_VERSION_MAJOR 0 )
set( papas_VERSION_MINOR 1 )
set( papas_VERSION_PATCH 0 )
set( papas_VERSION "${papas_VERSION_MAJOR}.${papas_VERSION_MINOR}" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif(APPLE)

#--- Declare options -----------------------------------------------------------
option(papas_documentation "Whether or not to create doxygen doc target." ON)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} $ENV{PODIO} $ENV{FCCEDM})

#--- C++ Standard --------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

if(NOT CMAKE_CXX_STANDARD MATCHES "14|17")
  message(FATAL_ERROR "Unsupported C++ standard: ${CMAKE_CXX_STANDARD}")
endif()

message (STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD}\ -stdlib=libc++")
else() 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD}\ -latomic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL ${CPP_STANDARD_FLAGS} -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter")
endif(APPLE)

#--- Declare ROOT dependency ---------------------------------------------------
if(APPLE)
  set(CMAKE_MACOSX_RPATH 1)
  #set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)
  find_package(ROOT REQUIRED COMPONENTS RIO Tree)
  include_directories(${ROOT_INCLUDE_DIRS})
  add_definitions(${ROOT_CXX_FLAGS})
  include("${ROOT_USE_FILE}")
else()
  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
  find_package(ROOT REQUIRED COMPONENTS RIO Tree Graf)
  include_directories(${ROOT_INCLUDE_DIR})
  include(${ROOT_USE_FILE})
endif(APPLE)

#---PODIO and FCC-EDM-----------------------------------------------------------
find_package(podio REQUIRED HINTS $ENV{PODIO})
message(STATUS "Found podio: ${podio_DIR}")
# Make sure the library is found.
# Not the case if LD_LIBRARY_PATH is wrong
find_library(PODIOLIB podio)
if (NOT PODIOLIB)
message(FATAL_ERROR "libpodio.so(dylib) cannot be found dynamically. Make sure you have sourced PODIO init*.sh file to set up your environment to use PODIO")
endif()
link_directories(${podio_LIBRARY_DIR})

find_package(fccedm REQUIRED HINTS $ENV{FCCEDM})
message(STATUS "Found fcc-edm: ${fccedm_DIR}")
link_directories(${fccedm_DIR})

include_directories(
"${podio_INCLUDE_DIR}"
"${FCCEDM_INCLUDE_DIRS}"
)

add_definitions(-Wno-unused-variable -Wno-unused-parameter -pthread)

#--- enable unit testing capabilities ------------------------------------------
include(CTest)

#--- target for Doxygen documentation ------------------------------------------
#if(papas_documentation)
    include(cmake/papasDoxygen.cmake)
#endif()

#--- add version files ---------------------------------------------------------
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/papasVersion.h
              ${CMAKE_CURRENT_BINARY_DIR}/papasVersion.h )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/papasVersion.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/papas)

#--- add CMake infrastructure --------------------------------------------------
include(cmake/papasCreateConfig.cmake)

#--- add license files ---------------------------------------------------------
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
              ${CMAKE_CURRENT_SOURCE_DIR}/NOTICE
        DESTINATION ${CMAKE_INSTALL_PREFIX})

add_subdirectory(papaslib)
add_subdirectory(examples)
add_subdirectory(tests)
#add_subdirectory(graphtools)
#add_subdirectory(dageg)

#
#this test only if R/Rcpp/Rinside are installed version 3.2+
#note there are a lot of R related warnings (originating from R)
#add_subdirectory(tests/rtest)
