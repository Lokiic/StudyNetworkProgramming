cmake_minimum_required(VERSION 3.0)

project(TCP_reliable)

include_directories(../lib)

add_executable(r_client1 reliable_client01.c)

add_executable(r_server1 reliable_server01.c)

add_executable(r_client2 reliable_client02.c)

add_executable(r_server2 reliable_server02.c)