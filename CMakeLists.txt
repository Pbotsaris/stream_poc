cmake_minimum_required(VERSION 3.7)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug) # debug build

project(streams)


find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)
find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)
find_path(RESAMPLE_INCLUDE_DIR libswresample/swresample.h)
find_library(RESAMPLE_LIBRARY swresample)
find_path(SCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SCALE_LIBRARY swscale)





find_package(SDL2 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SRCS "*.cpp")

add_executable(streams ${SRCS})
target_include_directories(streams  PRIVATE ${AVCODEC_INCLUDE_DIR} 
                                    ${AVFORMAT_INCLUDE_DIR} 
                                    ${AVUTIL_INCLUDE_DIR} 
                                    ${AVDEVICE_INCLUDE_DIR}
                                    ${RESAMPLE_INCLUDE_DIR}
                                    ${SCALE_INCLUDE_DIR}
                                    )

                                    
target_link_libraries(streams ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${SCALE_LIBRARY}  ${RESAMPLE_LIBRARY} ${SDL2_LIBRARIES} ${OpenCV_LIBS})

