add_executable(cool main.cpp)

target_link_libraries(cool ${GTKMM_LIBRARIES} cool_gl)

include(GNUInstallDirs)

install(TARGETS cool
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
