cmake_minimum_required(VERSION 3.24.0.0)

project(QSQLite3MultipleCiphers LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS CorePrivate SqlPrivate)

qt_add_plugin(${PROJECT_NAME}
    SHARED
    PLUGIN_TYPE sqldrivers
    qsql_sqlite.cpp
    qsql_sqlite_p.h
    qsql_sqlite_vfs.cpp
    qsql_sqlite_vfs_p.h
    qsql_sqlite_multiple_ciphers.cpp
    qsql_sqlite_multiple_ciphers_p.h
    smain.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_NO_AS_CONST
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_CONTEXTLESS_CONNECT
    QT_NO_EXCEPTIONS
    QT_NO_FOREACH
    QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    QT_NO_QEXCHANGE
    QT_USE_QSTRINGBUILDER
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::CorePrivate
    Qt6::SqlPrivate
)

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../sqldrivers)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLite3MultipleCiphers/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SQLite3MultipleCiphers)
target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3mc_static)
