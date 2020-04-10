find_package(pybind11 REQUIRED)

pybind11_add_module(cppimport cppimport.cc)
target_link_libraries(cppimport PUBLIC base)
install(TARGETS cppimport DESTINATION ${CMAKE_INSTALL_PREFIX}/python/lsst/base)

pybind11_add_module(threads threads.cc)
target_link_libraries(threads PUBLIC base)
install(TARGETS threads DESTINATION ${CMAKE_INSTALL_PREFIX}/python/lsst/base)

pybind11_add_module(versions versions.cc)
target_link_libraries(versions PUBLIC base)
install(TARGETS versions DESTINATION ${CMAKE_INSTALL_PREFIX}/python/lsst/base)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.py CONTENT "\
__version__ = 'unknown'
__all__ = ('__version__',)
")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/version.py DESTINATION ${CMAKE_INSTALL_PREFIX}/python/lsst/base)
