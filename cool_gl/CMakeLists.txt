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
project(cool_gl VERSION 0.0.1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)

# Set target name
set(target_name cool_gl)

# Set export variables
set(targets_export_name ${PROJECT_NAME}-exports)
set(export_namespace_name ${PROJECT_NAME}::)

###############################################################################
# Create target
###############################################################################

# Set Library sources
set(sources
    src/Drawable.cpp
    src/clipping.cpp
    src/Line.cpp
    src/Matrix.cpp
    src/Point.cpp
    src/Polygon.cpp
    src/Vec.cpp
    src/Window.cpp
)

# Set Library public headers
set(public_headers
    include/cool_gl/Colour.h
    include/cool_gl/cool_gl.h
    include/cool_gl/Drawable.h
    include/cool_gl/Line.h
    include/cool_gl/Matrix.h
    include/cool_gl/Point.h
    include/cool_gl/Polygon.h
    include/cool_gl/Vec.h
    include/cool_gl/Window.h
)

add_library(${target_name} ${sources})
add_library(${export_namespace_name}${target_name} ALIAS ${target_name})

target_compile_features(${target_name} PUBLIC cxx_std_11)

set_target_properties(${target_name} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    PUBLIC_HEADER ${public_headers}
)

target_include_directories(${target_name} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${target_name} PkgConfig::GTKMM)

###############################################################################
# Test Library
###############################################################################
enable_testing()
add_subdirectory(test)

###############################################################################
# Install rules
###############################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

export(TARGETS ${target_name}
    NAMESPACE ${export_namespace_name}
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
    LIBRARY
        COMPONENT Libraries
        NAMELINK_COMPONENT Development
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER
        COMPONENT Development
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
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
    NAMESPACE ${export_namespace_name}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
)
