cmake_minimum_required(VERSION 3.1.0)

project(ServerTcp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_executable(ServerTcp
    ServerTcp.ui
    ServerTcp.cpp
    main.cpp
    ServerTcp.qrc
)

target_link_libraries(ServerTcp Qt5::Widgets Qt5::Network)