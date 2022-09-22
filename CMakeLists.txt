cmake_minimum_required(VERSION 3.7)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

project(streams)

# ADDING FFMPG manually
INCLUDE_DIRECTORIES(/usr/include/ffmpeg4.4/libavcodec)
INCLUDE_DIRECTORIES(/usr/include/ffmpeg4.4/libavformat)
INCLUDE_DIRECTORIES(/usr/include/ffmpeg4.4/libavutil)
LINK_DIRECTORIES(/usr/lib/ffmpeg4.4)

find_package(SDL2 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SRCS "*.cpp")

add_executable(streams ${SRCS})
target_link_libraries(streams -lavformat -lavcodec -lavutil)
target_link_libraries(streams ${SDL2_LIBRARIES} ${OpenCV_LIBS})

