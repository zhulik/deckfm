find_package(Qt5 REQUIRED COMPONENTS Qml)

include_directories(${CMAKE_SOURCE_DIR})

# FIXME: use current path
configure_file(${CMAKE_SOURCE_DIR}/tools/vdf/vdf.js ${CMAKE_CURRENT_BINARY_DIR}/vdf.js COPYONLY)

file(GLOB_RECURSE sources "*.cpp")
set(PROJECT_SOURCES ${sources} #[[qml.qrc]])

add_executable(vdf ${PROJECT_SOURCES})
target_link_libraries(vdf PRIVATE Qt5::Qml)
