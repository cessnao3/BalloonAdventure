cmake_minimum_required(VERSION 3.5)

project(iolwge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(iolwge STATIC)

target_sources(iolwge
    PRIVATE
        draw_object.cpp
        game_object.cpp
        physics_object.cpp
        step_object.cpp
        vector2.cpp
    )

target_include_directories(iolwge PUBLIC .)
