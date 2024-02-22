cmake_minimum_required(VERSION 3.15)
project(common)

aux_source_directory(./ SRC)
aux_source_directory(./json/ SRC_JSON)

list(APPEND SRC ${SRC_JSON})

add_library(common STATIC ${SRC})