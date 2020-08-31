cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(server)

set(OpenCV_DIR /home/hkuit104/opencv-3.4.6/include)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#SET(PLATFORM_SPECIFIC_LIBS "-lpthread")

aux_source_directory(. DIR_SRCS)

add_executable(server ${DIR_SRCS})
target_link_libraries(server "${OpenCV_LIBS}" -lpthread)
set_property(TARGET server PROPERTY CXX_STANDARD 11)
