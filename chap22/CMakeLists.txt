cmake_minimum_required(VERSION 3.0)

project(non-block)

include_directories(../lib)

add_executable(nonblocking_c nonblocking_client.c)

add_executable(nonblocking_s nonblocking_server.c)

add_executable(ans_s ans_server.c)

add_executable(test_c test_client.c)