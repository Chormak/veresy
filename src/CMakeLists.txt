cmake_minimum_required(VERSION 3.16)
project(Veresy)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(veresy 
main.cpp
app/Application.cpp
app/Application.h
ui/MainWindow.cpp
ui/MainWindow.h
)

target_link_libraries(veresy PRIVATE Qt6::Widgets)
