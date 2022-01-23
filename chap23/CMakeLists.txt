cmake_minimum_required(VERSION 3.0)

project(EPOLL_STUDY)

include_directories(../lib)

add_executable(ep1_s epoll1_server.c)

add_executable(ep2_s epoll2_server.c)

add_executable(ep3_s epoll3_server.c)

add_executable(test_c test_client.c)