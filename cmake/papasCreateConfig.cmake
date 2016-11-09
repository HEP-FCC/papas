include(CMakePackageConfigHelpers)
configure_file(cmake/papasConfig.cmake.in "${PROJECT_BINARY_DIR}/papasConfig.cmake" @ONLY)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/papasConfigVersion.cmake
                                 VERSION ${papas_VERSION}
                                 COMPATIBILITY SameMajorVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/papasConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/papasConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake )
