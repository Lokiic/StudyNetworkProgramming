cmake_minimum_required(VERSION 3.0)

project(POLLSERVER)

include_directories(../lib)

add_executable(poll_s pollserver.c)

add_executable(test_c test_client.c)

