cmake_minimum_required(VERSION 3.0)

project(SELECT_STUDY)

include_directories(../lib)

add_executable(ep1_s epoll1_server.c)

add_executable(se_c select_client.c)