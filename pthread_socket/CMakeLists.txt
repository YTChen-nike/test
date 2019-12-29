cmake_minimum_required(VERSION 3.1.0)

add_executable(server server.cpp)
target_link_libraries(server pthread)

add_executable(client client.cpp)

