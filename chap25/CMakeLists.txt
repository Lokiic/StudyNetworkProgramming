cmake_minimum_required(VERSION 3.0)

project(blockio_process)

include_directories(../lib)

add_executable(fork_s fork_server.c)

add_executable(test_c test_client.c)