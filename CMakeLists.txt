cmake_minimum_required(VERSION 3.19)

project(Collision)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(MyApp main.cpp animation.cpp ball.cpp collision.cpp)

target_link_libraries(MyApp Qt6::Widgets)
