cmake_minimum_required(VERSION 3.0)

project(BUFFER)

include_directories(../lib)

add_executable(b_server1 buffer_server1.c)

add_executable(b_client buffer_client.c)

add_executable(b_server2 buffer_server2.c)