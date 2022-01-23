cmake_minimum_required(VERSION 3.0)

project(UNIX_SOCKET)

include_directories(../lib)

add_executable(un_stream_server unix_stream_server.c)

add_executable(un_stream_client unix_stream_client.c)

add_executable(un_datagram_server unix_datagram_server.c)

add_executable(un_datagram_client unix_datagram_client.c)