cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(client)

set(OpenCV_DIR /home/hkuit104/opencv-3.4.6/include)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(PLATFORM_SPECIFIC_LIBS "-lpthread")

aux_source_directory(. DIR_SRCS)

add_executable(client ${DIR_SRCS})
target_link_libraries(client "${OpenCV_LIBS}" -lpthread)
set_property(TARGET client PROPERTY CXX_STANDARD 11)
