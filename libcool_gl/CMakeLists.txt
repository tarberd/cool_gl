add_library(cool_gl src/cool_gl.cpp)

target_include_directories(cool_gl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(cool_gl ${GTKMM_LIBRARIES})

add_subdirectory(test)

include(GNUInstallDirs)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
)

install(DIRECTORY include/cool_gl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS cool_gl
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    EXPORT ${PROJECT_NAME}-targets
)

