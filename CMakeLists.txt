cmake_minimum_required(VERSION 3.5)

project(Client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS WebSockets)

add_executable(Client
  main.cpp
  client.h
  client.cpp
)
target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)

install(TARGETS Client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
