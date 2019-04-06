find_package(Catch2)

add_executable(spec test.cpp)

target_link_libraries(spec Catch2::Catch2 cool_gl)
