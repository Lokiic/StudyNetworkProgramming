cmake_minimum_required(VERSION 3.0)

project(PING-PONG)

include_directories(../lib .)

add_executable(pingpong_server ping-pong_server.c)

add_executable(pingpong_client ping-pong_client.c)

