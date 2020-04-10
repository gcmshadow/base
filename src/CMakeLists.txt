add_library(base SHARED)

target_compile_features(base PUBLIC cxx_std_14)
set_target_properties(base PROPERTIES 
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(base PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_sources(base PRIVATE
    library.cc
    ModuleImporter.cc
    threads.cc
    versions.cc
)

target_link_libraries(base PUBLIC
    dl
)

install(TARGETS base)
