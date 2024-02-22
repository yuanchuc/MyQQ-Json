cmake_minimum_required(VERSION 3.15)
project(EasyTcpServer)

# mysql数据库
set(MYSQL_LIB_PATH /usr/lib64/mysql)
set(MYSQL_INCLUDE_PATH /usr/include/mysql)
set(MYSQL_LIB mysqlclient)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/mysql
    ${PROJECT_SOURCE_DIR}/server
)

add_definitions("-Wall -g")

add_subdirectory(common)
add_subdirectory(mysql)
add_subdirectory(server)

link_directories(${MYSQL_LIB_PATH})
link_libraries(common mysql server)
add_executable(serve server.cpp)

target_link_libraries(serve ${MYSQL_LIB} pthread)