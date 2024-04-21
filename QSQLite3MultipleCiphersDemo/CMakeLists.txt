cmake_minimum_required(VERSION 3.14)

project(QSQLite3MultipleCiphersDemo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

qt_add_executable(${PROJECT_NAME}
    Main.cpp
    QSQLite3MultipleCiphersDemo.h
    QSQLite3MultipleCiphersDemo.cpp
    QSQLite3MultipleCiphersDemo.qrc
    QSQLite3MultipleCiphersDemo.ui
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../QSQLite3MultipleCiphers QSQLite3MultipleCiphers)
