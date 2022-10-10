cmake_minimum_required(VERSION 3.14)

project(deckfm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 REQUIRED COMPONENTS Core Quick QuickControls2 Gamepad)

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE sources "src/*.cpp")

set(PROJECT_SOURCES ${sources} qml.qrc)

add_executable(deckfm ${PROJECT_SOURCES})

configure_file(${CMAKE_SOURCE_DIR}/steam_appid.txt ${CMAKE_CURRENT_BINARY_DIR}/steam_appid.txt COPYONLY)

target_link_libraries(deckfm PRIVATE Qt5::Core Qt5::Quick Qt5::QuickControls2 Qt5::Gamepad ${CMAKE_SOURCE_DIR}/libsteam_api.so)
