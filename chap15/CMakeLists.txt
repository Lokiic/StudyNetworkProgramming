cmake_minimum_required(VERSION 3.0)

project(TEST_SO_REUSEADDR)

include_directories(../lib)

add_executable(noset_server noset_REUSEADDR_server.c)

add_executable(set_server set_REUSEADDR_server.c)

add_executable(test-client test_client.c)