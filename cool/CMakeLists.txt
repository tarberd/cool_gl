add_custom_target(glade_file
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/cool_glade.glade
        ${CMAKE_CURRENT_BINARY_DIR}/cool_glade.glade
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(cool main.cpp)

add_dependencies(cool glade_file)

target_link_libraries(cool ${GTKMM_LIBRARIES} cool_gl)

include(GNUInstallDirs)

install(TARGETS cool
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
