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
if (Boost_FOUND)
include_directories( ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/DAGtool )
endif()

include_directories(${CMAKE_SOURCE_DIR}/fastsim ${CMAKE_SOURCE_DIR}/utility ${CMAKE_SOURCE_DIR}/detectors ${CMAKE_SOURCE_DIR}/display
${CMAKE_SOURCE_DIR}/datatypes ${CMAKE_SOURCE_DIR}/simulation  )

set (PAPAS_ROOT /Users/alice/fcc/cpp/papas/src/)
#R support
if (0)
set (RCPP_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/library/Rcpp/include)
set (RI_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/include)
set (R_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/include)
set (RLIB_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/lib)
set (RINSIDE_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/lib)
include_directories(  ${RI_ROOT}  )
include_directories(  ${RCPP_ROOT}  )
include_directories(  ${R_ROOT}  )
set (DYLD_LIBRARY_PATH /Users/alice/local/lib/root  $RLIB_ROOT $RINSIDE_ROOT)
target_link_libraries(mainexe fastsim ${ROOT_LIBRARIES} /Library/Frameworks/R.framework/Versions/3.2/Resources/lib/libR.dylib  /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/lib/libRInside.dylib)
add_library(RInside SHARED IMPORTED)
add_library(R SHARED IMPORTED)
add_executable(mainexe ../main.cpp)
target_link_libraries(mainexe fastsim ${ROOT_LIBRARIES} /Library/Frameworks/R.framework/Versions/3.2/Resources/lib/libR.dylib  /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/lib/libRInside.dylib)
else()
set (DYLD_LIBRARY_PATH /Users/alice/local/lib/root )
add_executable(mainexe ../main.cpp)
target_link_libraries(mainexe fastsim ${ROOT_LIBRARIES} )
endif()



#add_subdirectory(../tests)

