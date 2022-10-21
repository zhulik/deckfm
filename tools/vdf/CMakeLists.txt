find_package(Qt5 REQUIRED COMPONENTS Qml)

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE sources "*.cpp")

# TODO: export vdf.js to a separate qrc file and with link with it
set(PROJECT_SOURCES ${sources})

add_executable(vdf ${PROJECT_SOURCES})
target_link_libraries(vdf PRIVATE Qt5::Qml)
