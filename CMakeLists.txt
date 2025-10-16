cmake_minimum_required(VERSION 3.21)
project(MyHttp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators")

find_package(Boost REQUIRED CONFIG)
find_package(GTest REQUIRED CONFIG)

enable_testing()
add_executable(MyHttpTests test/HttpRequestFactoryTests.cpp)
add_test(NAME MyHttpTests COMMAND MyHttpTests)

add_library(MyHttp src/MyHttpLib/include/TcpConnection.h src/MyHttpLib/include/HttpRequestFactory.h src/MyHttpLib/src/TcpConnection.cpp src/MyHttpLib/src/HttpRequestFactory.cpp
            src/MyHttpLib/include/ResourceManager.h src/MyHttpLib/src/ResourceManager.cpp src/MyHttpLib/src/HttpServer.cpp src/MyHttpLib/src/TcpServer.cpp src/MyHttpLib/include/TcpServer.h src/MyHttpLib/src/Utils.cpp)

add_executable(simple_http_server src/server.cpp)
add_executable(simple_http_client src/client.cpp)


target_include_directories(MyHttpTests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MyHttpLib/src
)
target_include_directories(MyHttp
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MyHttpLib/include
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)
target_include_directories(simple_http_server
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)
target_include_directories(simple_http_client
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(MyHttp PRIVATE Boost::boost Boost::filesystem)
target_link_libraries(simple_http_server PUBLIC MyHttp 
                                         PRIVATE Boost::boost Boost::filesystem)
target_link_libraries(simple_http_client PUBLIC MyHttp
                                         PRIVATE Boost::boost)

target_link_libraries(MyHttpTests PUBLIC MyHttp PRIVATE GTest::gtest_main)