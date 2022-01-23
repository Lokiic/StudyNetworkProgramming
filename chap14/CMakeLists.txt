cmake_minimum_required(VERSION 3.0)

project(UDPCONNECT)

include_directories(../lib)

add_executable(udp-connect-server udp_connect_server.c)

add_executable(udp-connect-client udp_connect_client.c)