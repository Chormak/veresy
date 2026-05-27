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
ui/orders/OrdersView.h
ui/orders/OrdersView.cpp
ui/filters/FiltersPanel.h
ui/filters/FiltersPanel.cpp
ui/orders/OrderDetailsView.h
ui/orders/OrderDetailsView.cpp
ui/common/AppToolbar.h
ui/common/AppToolbar.cpp
ui/screens/InventoryScreen.h
ui/screens/FinanceScreen.h
ui/screens/AnalyticsScreen.h
utils/Logger.h
utils/Logger.cpp
ui/orders/WorkflowListWidget.h
ui/orders/WorkflowListWidget.cpp
)

target_link_libraries(veresy PRIVATE Qt6::Widgets Qt6::Sql)
