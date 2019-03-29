find_package(Catch2)

add_executable(test test.cpp)

target_link_libraries(test Catch2::Catch2 cool_gl)
