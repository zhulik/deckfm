cmake_minimum_required(VERSION 3.14)

project(deckfm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Quick QuickControls2)

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/QSteamworks)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

file(GLOB_RECURSE RESOURCES "resources/*.*")
add_custom_target(resources SOURCES ${RESOURCES})

set(PROJECT_SOURCES ${SOURCES})

add_subdirectory(tools/vdf)

configure_file(${CMAKE_SOURCE_DIR}/steam_appid.txt ${CMAKE_CURRENT_BINARY_DIR}/steam_appid.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/input.vdf ${CMAKE_CURRENT_BINARY_DIR}/input.vdf COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/steam_controller.vdf ${CMAKE_CURRENT_BINARY_DIR}/steam_controller.vdf COPYONLY)

add_executable(deckfm ${PROJECT_SOURCES})
target_link_libraries(deckfm PRIVATE Qt5::Core Qt5::Quick Qt5::QuickControls2 ${CMAKE_SOURCE_DIR}/libsteam_api.so)

###################### INSTALL

install(TARGETS deckfm DESTINATION deckfm)
install(FILES libsteam_api.so steam_appid.txt input.vdf DESTINATION deckfm)
install(PROGRAMS deckfm.sh DESTINATION deckfm)
install(DIRECTORY resources DESTINATION deckfm)
