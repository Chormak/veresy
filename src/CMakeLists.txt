cmake_minimum_required(VERSION 3.16)
project(Veresy)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(veresy 
main.cpp
app/Application.cpp
app/Application.h
ui/MainWindow.cpp
ui/MainWindow.h
data/Database.cpp
data/Database.h
core/orders/Order.h
core/orders/OrderManager.cpp
core/orders/OrderManager.h
ui/OrderDialog.h
ui/OrderDialog.cpp
data/repositories/OrderRepository.cpp
data/repositories/OrderRepository.h
ui/OrderTableModel.h
ui/OrderTableModel.cpp
ui/OrderTableView.h
ui/OrderTableView.cpp
)

target_link_libraries(veresy PRIVATE Qt6::Widgets Qt6::Sql)
