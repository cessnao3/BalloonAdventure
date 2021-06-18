cmake_minimum_required(VERSION 3.5)

project(balloon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO
    REQUIRED
        allegro-5
        allegro_primitives-5)

add_executable(balloon src/main.cpp)
target_include_directories(balloon PUBLIC src/)
#target_include_directories(balloon PUBLIC .)

add_subdirectory(iolwge)
target_link_libraries(balloon iolwge)

target_link_libraries(balloon ${ALLEGRO_LINK_LIBRARIES})
target_include_directories(balloon PRIVATE ${ALLEGRO_INCLUDE_DIRS})
target_link_directories(balloon PRIVATE ${ALLEGRO_LIBRARY_DIRS})