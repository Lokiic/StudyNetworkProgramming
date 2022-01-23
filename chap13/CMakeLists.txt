cmake_minimum_required(VERSION 3.0)

project(writev)

include_directories(../lib)

add_executable(writev_client writev_client.c)

add_executable(testserver test-server.c)

