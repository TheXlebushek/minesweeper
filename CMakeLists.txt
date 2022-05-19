cmake_minimum_required(VERSION 3.20)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_compile_definitions(_HAS_AUTO_PTR_ETC=1)
endif()
add_subdirectory(vendor/SFML)

add_executable(minesweeper WIN32 src/main.cpp src/Field.cpp src/Field.h src/Tile.cpp src/Tile.h)

target_link_libraries(minesweeper sfml-graphics opengl32)

configure_file(vendor/SFML/extlibs/bin/x64/openal32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})