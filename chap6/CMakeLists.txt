cmake_minimum_required(VERSION 3.0)

project(udp_server_client)

include_directories(../lib)

add_executable(udpserver udp_server.c)

add_executable(udpclient udp_client.c)

