cmake_minimum_required(VERSION 3.25)
project(CARnary_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(CARnary_server src/main.cpp src/Negotiation.cpp src/CARnaryServer.cpp src/Logger.cpp src/Watcher.cpp)
target_link_libraries(CARnary_server PRIVATE pthread)
