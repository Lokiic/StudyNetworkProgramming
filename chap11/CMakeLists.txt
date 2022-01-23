cmake_minimum_required(VERSION 3.0)

project(CLOSE_SHUTDOWN_TEST)

include_directories(../lib)

add_executable(graceserver close_shutdown_server.c)

add_executable(graceclient close_shutdown_client.c)
