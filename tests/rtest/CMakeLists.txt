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

set (MAINDIR "${CMAKE_SOURCE_DIR}/papascpp")
include_directories(
${MAINDIR}/utility/utility
${MAINDIR}/detectors/detectors
${MAINDIR}/display/display
${MAINDIR}/datatypes/datatypes
${MAINDIR}/simulation/simulation
${MAINDIR}/graphtools/graphtools)

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

#set (DYLD_LIBRARY_PATH /Users/alice/local/root/lib)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS} /usr/local/include /usr/local/lib )

#message(${ROOT_LIBRARIES})


set (RCPP_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/library/Rcpp/include)
set (RI_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/include)
set (R_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/include)
set (RLIB_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/lib)
set (RINSIDE_ROOT /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/lib)
include_directories(  ${RI_ROOT}  )
include_directories(  ${RCPP_ROOT}  )
include_directories(  ${R_ROOT}  )
set (DYLD_LIBRARY_PATH  $RLIB_ROOT $RINSIDE_ROOT)






add_executable(rtest  r_test.cpp )
target_link_libraries(rtest  papascpp ${ROOT_LIBRARIES} /Library/Frameworks/R.framework/Versions/3.2/Resources/lib/libR.dylib  /Library/Frameworks/R.framework/Versions/3.2/Resources/library/RInside/lib/libRInside.dylib ${GTEST_LIBRARIES})
add_library(RInside SHARED IMPORTED)
add_library(R SHARED IMPORTED)




