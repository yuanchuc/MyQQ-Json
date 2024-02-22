cmake_minimum_required(VERSION 3.15)
project(mysql)

include_directories(${MYSQL_INCLUDE_PATH})

aux_source_directory(./ SRC)

link_directories(${MYSQL_LIB_PATH})

add_library(mysql ${SRC})