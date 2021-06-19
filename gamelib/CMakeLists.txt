cmake_minimum_required(VERSION 3.5)

project(gamelib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gamelib STATIC)

target_sources(gamelib
    PRIVATE
        draw_object.cpp
        game_object.cpp
        physics_object.cpp
        step_object.cpp
        vector2.cpp
    )
	
target_compile_options(gamelib PRIVATE -Wall -Wextra -pedantic -Werror)
target_include_directories(gamelib PUBLIC .)
