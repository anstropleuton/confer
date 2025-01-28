cmake_minimum_required(VERSION 3.30)

project(confer
    VERSION 2.1.0.0
    DESCRIPTION "Confer is a testing framework for Anstro Pleuton's libraries and programs."
    LANGUAGES CXX
)

option(CONFER_BUILD_TESTS "Build Confer tests" OFF)
option(CONFER_BUILD_EXAMPLES "Build Confer examples" OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/confer_config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/confer_config.hpp"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkgconfig.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/confer.pc"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/header.html.in"
    "${CMAKE_CURRENT_BINARY_DIR}/header.html"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/footer.html.in"
    "${CMAKE_CURRENT_BINARY_DIR}/footer.html"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/conferConfig.cmake"
    @ONLY
)

# Taken from GLFW
# Only generate this target if no higher-level project already has
if (NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    )
endif()

set(CONFER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/confer.cpp"
)
set(CONFER_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/confer_config.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/confer.hpp"
)
set(CONFER_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

add_library(confer)
target_compile_features(confer PUBLIC cxx_std_23)
target_sources(confer PRIVATE ${CONFER_SOURCES})
target_sources(confer PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CONFER_INCLUDE_DIRS}
    FILES ${CONFER_HEADERS}
)

if(CONFER_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(CONFER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/confer.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/conferConfigVersion.cmake"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/conferConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/conferConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/confer"
)
install(
    TARGETS confer
    EXPORT conferTargets
    FILE_SET HEADERS
)
install(
    EXPORT conferTargets
    NAMESPACE confer::
    DESTINATION "${CMAKE_INSTALL_DATADIR}/confer"
)
