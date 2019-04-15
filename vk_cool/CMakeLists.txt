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
project(vk_cool VERSION 0.0.1)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.2 REQUIRED)


find_package(cool_gl REQUIRED)

set(target_name vk_cool)

###############################################################################
# Create Target
###############################################################################
add_custom_command(
    OUTPUT vert.spv
    COMMAND glslangValidator -V ${PROJECT_SOURCE_DIR}/src/shader.vert -o ${PROJECT_BINARY_DIR}/vert.spv
)

add_custom_command(
    OUTPUT frag.spv
    COMMAND glslangValidator -V ${PROJECT_SOURCE_DIR}/src/shader.frag -o ${PROJECT_BINARY_DIR}/frag.spv
    DEPENDS ${PROJECT_BINARY_DIR}/vert.spv
)

add_custom_target(
    shaders
    DEPENDS ${PROJECT_BINARY_DIR}/vert.spv
    DEPENDS ${PROJECT_BINARY_DIR}/frag.spv
)

set(sources
    src/main.cpp
)

add_executable(${target_name} ${sources})

target_compile_features(${target_name} PUBLIC cxx_std_17)

add_dependencies(${target_name} glade_file)

target_link_libraries(${target_name} Vulkan::Vulkan glfw)
add_dependencies(${target_name} shaders)

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

