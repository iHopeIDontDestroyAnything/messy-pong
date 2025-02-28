cmake_minimum_required(VERSION 3.15)
project(test3)
find_package(SDL2)
set(SOURCE_FILES
  main.cpp
  gameOBJs/player.cpp
  gameOBJs/ball.cpp)

add_executable(run ${SOURCE_FILES})
target_link_libraries(run SDL2)
