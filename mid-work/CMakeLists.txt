cmake_minimum_required(VERSION 3.0)

project(mid-homework)

include_directories(../lib)

add_executable(mid-client mid_client.c)

add_executable(mid-server mid_server.c)