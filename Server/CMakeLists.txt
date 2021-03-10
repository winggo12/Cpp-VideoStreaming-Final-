cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(server)

#SET(PLATFORM_SPECIFIC_LIBS )

aux_source_directory(. DIR_SRCS)

add_executable(server ${DIR_SRCS})
target_link_libraries(server -lpthread)
set_property(TARGET server PROPERTY CXX_STANDARD 11)
