cmake_minimum_required(VERSION 3.0)

project(TCP_CLIENT_SERVER)

include_directories(/home/manu/HTTPserver/lib)

add_executable(tcpclient tcp_client.c)
add_executable(tcpserver tcp_server.c)