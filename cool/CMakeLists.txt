cmake_minimum_required(VERSION 3.7.2)

###############################################################################
# Set default CMAKE_BUILD_TYPE
###############################################################################
if(NOT CMAKE_BUILD_TYPE)
    set(default_build_type "Release")
    string(CONCAT message
        "Setting CMAKE_BUILD_TYPE to '${default_build_type}' "
        "as none was specified."
    )
    message(STATUS ${message})
    string(CONCAT out
        "Choose the type of build, options are: "
        "None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) "
        "Debug Release RelWithDebInfo MinSizeRel."
    )
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING ${out} FORCE)
endif()

###############################################################################
# Create Project
###############################################################################
project(cool VERSION 0.0.1)

find_package(PkgConfig)
pkg_check_modules(GTKMM IMPORTED_TARGET gtkmm-3.0)

find_package(cool_gl REQUIRED)

# Set target name
set(target_name cool)

###############################################################################
# Create Target
###############################################################################
add_custom_target(glade_file ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_SOURCE_DIR}/data/cool_app_window_design.glade
    ${PROJECT_BINARY_DIR}/cool_app_window_design.glade
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

# Set sources
set(sources
    src/main.cpp
    src/cool_app.cpp
)

add_executable(${target_name} ${sources})

target_compile_features(${target_name} PRIVATE cxx_std_17)

add_dependencies(${target_name} glade_file)

target_link_libraries(${target_name} PkgConfig::GTKMM cool_gl::cool_gl)

###############################################################################
# Test
###############################################################################
# enable_testing()
# add_subdirectory(test)

###############################################################################
# Install rules
###############################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(targets_export_name ${PROJECT_NAME}-exports)
export(TARGETS ${target_name}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_BINARY_DIR}/${targets_export_name}.cmake
)

write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/package-config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIRA}/${PROJECT_NAME}/cmake
)

install(TARGETS ${target_name}
    EXPORT ${targets_export_name}
    RUNTIME
        COMPONENT Runtime
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    COMPONENT Development
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
)

install(EXPORT ${targets_export_name}
    COMPONENT Development
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
)

