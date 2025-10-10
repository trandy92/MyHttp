cmake_minimum_required(VERSION 3.21)
project(SimpleHttpServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan setup
# include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators")

find_package(Boost REQUIRED CONFIG)

add_executable(simple_http_server src/main_server.cpp)
add_executable(simple_http_client src/main_client.cpp)

target_include_directories(simple_http_server
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(simple_http_server PRIVATE Boost::boost Boost::filesystem)
target_include_directories(simple_http_client
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(simple_http_client PRIVATE Boost::boost)