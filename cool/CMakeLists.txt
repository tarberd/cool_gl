add_executable(cool main.cpp)

target_link_libraries(cool ${GTKMM_LIBRARIES} cool_gl)

include(GNUInstallDirs)

install(TARGETS cool
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

execute_process(
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/cool_glade.glade ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE result
)
