cmake_minimum_required(VERSION 3.0)

project(STREAM)

include_directories(../lib)

add_executable(streamserver stream_server.c)

add_executable(streamclient stream_client.c)