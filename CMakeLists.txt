cmake_minimum_required(VERSION 2.8)

project(papas)
set( papas_VERSION_MAJOR 0 )
set( papas_VERSION_MINOR 1 )
set( papas_VERSION_PATCH 0 )
set( papas_VERSION "${papas_VERSION_MAJOR}.${papas_VERSION_MINOR}" )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

    #tell mac to use RPATH
	set(CMAKE_MACOSX_RPATH 1)
endif(APPLE)

#--- Declare options -----------------------------------------------------------
option(papas_documentation "Whether or not to create doxygen doc target.")


list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} $ENV{PODIO} $ENV{FCCEDM})

#--- Declare ROOT dependency ---------------------------------------------------
#NB on the mac I think we need to find the ROOT CMake not the local one which I therefore commented out in the cmake directory
#need to understand what is the problem
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(ROOT REQUIRED)
add_definitions(${ROOT_CXX_FLAGS}) 
include("${ROOT_USE_FILE}") #important for Mac Xcode

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

include_directories(
"${podio_INCLUDE_DIRS}"
"${FCCEDM_INCLUDE_DIRS}"
)


add_definitions(-Wno-unused-variable -Wno-unused-parameter -pthread)
#--temporary fix of inconsistency in ROOT CMake macros
set(ROOT_genreflex_cmd ${ROOT_genreflex_CMD})


#--- enable unit testing capabilities ------------------------------------------
include(CTest)

#--- target for Doxygen documentation ------------------------------------------
if(papas_documentation)
    include(cmake/papasDoxygen.cmake)
endif()

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


add_subdirectory(papascpp)
#add_subdirectory(papascpp/graphtools)
add_subdirectory(main)
add_subdirectory(examples)
add_subdirectory(papaslib)
#
#this test only if R/Rcpp/Rinside are installed version 3.2+
#note there are a lot of R related warnings (originating from R)
#add_subdirectory(tests/rtest)

add_subdirectory(tests)



