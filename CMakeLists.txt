cmake_minimum_required(VERSION 3.25)
project(project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(project1 server.cpp)

# Link against the Winsock library
target_link_libraries(project1 ws2_32)
