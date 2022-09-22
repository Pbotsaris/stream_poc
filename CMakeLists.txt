cmake_minimum_required(VERSION 3.7)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

project(sdl)

INCLUDE_DIRECTORIES(/usr/local/Cellar/ffmpeg/5.1/include/)
LINK_DIRECTORIES(/usr/local/Cellar/ffmpeg/5.1/lib/)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SRCS "*.cpp")

add_executable(sdl ${SRCS})
target_link_libraries(sdl ${SDL2_LIBRARIES})
target_link_libraries(sdl -lavformat -lavcodec)

