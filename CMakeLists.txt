cmake_minimum_required(VERSION 3.25)
project(CARnary_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# find the client library
find_package(CARnary_lib REQUIRED)

add_executable(CARnary_server src/main.cpp src/CARnaryServer.cpp src/Logger.cpp src/Watcher.cpp
)
target_link_libraries(CARnary_server PRIVATE pthread CARnary_lib)
