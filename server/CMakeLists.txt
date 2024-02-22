cmake_minimum_required(VERSION 3.15)
project(server)



aux_source_directory(./ SRC)
link_libraries(common)

add_library(server STATIC ${SRC})