# adapted from https://github.com/jasujm/apparatus-examples/tree/master/docker-cpp/CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(GameAgent)

find_package(cppzmq)

add_executable(GameAgent main.cpp)
target_link_libraries(GameAgent cppzmq)
set_property(TARGET GameAgent PROPERTY CXX_STANDARD 17)

install(TARGETS GameAgent RUNTIME DESTINATION bin)