add_subdirectory(lsst/base)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/python
    PATTERN "CMakeLists.txt" EXCLUDE
    PATTERN "SConscript" EXCLUDE
    PATTERN "*.cc" EXCLUDE
)
