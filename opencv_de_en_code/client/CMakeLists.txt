cmake_minimum_required(VERSION 3.7)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

project(client)

find_package(SDL2 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SRCS "*.cpp")

add_executable(client ${SRCS})

target_link_libraries(client ${SDL2_LIBRARIES} ${OpenCV_LIBS})