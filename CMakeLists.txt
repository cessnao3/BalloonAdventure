cmake_minimum_required(VERSION 3.18)

project(BalloonAdventure VERSION 0.1 LANGUAGES CXX)
set(TARGET_NAME BalloonAdventure)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    lib/gamelib/aero_object.cpp
    lib/gamelib/aero_object.h
    lib/gamelib/constants.cpp
    lib/gamelib/constants.h
    lib/gamelib/draw_object.cpp
    lib/gamelib/draw_object.h
    lib/gamelib/game_object.h
    lib/gamelib/input_manager.cpp
    lib/gamelib/input_manager.h
    lib/gamelib/physics_object.cpp
    lib/gamelib/physics_object.h
    lib/gamelib/polygon.cpp
    lib/gamelib/polygon.h
    lib/gamelib/rectangle.cpp
    lib/gamelib/rectangle.h
    lib/gamelib/step_object.cpp
    lib/gamelib/step_object.h
    lib/gamelib/vector2.cpp
    lib/gamelib/vector2.h
    src/balloon/balloon.cpp
    src/balloon/balloon.h
    src/balloon/envelope.cpp
    src/balloon/envelope.h
    src/balloon/gondola.cpp
    src/balloon/gondola.h
    src/balloon/rope.cpp
    src/balloon/rope.h
    src/balloon/weight.cpp
    src/balloon/weight.h
    src/game_state.cpp
    src/game_state.h
    src/main.cpp
    src/menu_state_flow.cpp
    src/menu_state_flow.h
    src/sound_manager.cpp
    src/sound_manager.h
    src/terrain.cpp
    src/terrain.h
    src/world_state.h
)

add_executable(${TARGET_NAME} ${PROJECT_SOURCES})

find_library(ALLEGRO NAMES allegro REQUIRED)
find_library(ALLEGRO_AUDIO NAMES allegro_audio REQUIRED)
find_library(ALLEGRO_ACODEC NAMES allegro_acodec REQUIRED)
find_library(ALLEGRO_TTF NAMES allegro_ttf REQUIRED)
find_library(ALLEGRO_FONT NAMES allegro_font REQUIRED)
find_library(ALLEGRO_PRIMITIVES NAMES allegro_primitives REQUIRED)

target_link_libraries(${TARGET_NAME} PRIVATE "${ALLEGRO}" "${ALLEGRO_AUDIO}" "${ALLEGRO_ACODEC}" "${ALLEGRO_TTF}" "${ALLEGRO_FONT}" "${ALLEGRO_PRIMITIVES}")

target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib" "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -pedantic -Werror)
endif()
