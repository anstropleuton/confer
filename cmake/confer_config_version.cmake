# confer_config.cmake
include(${CMAKE_CURRENT_LIST_DIR}/confer_targets.cmake)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/confer_config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/confer_config_version.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/confer
)
