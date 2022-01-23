#include<stdio.h>  // 标准输入输出
#include<stdlib.h> // 标准库头文件,size_t, malloc()等
#include<sys/socket.h>  // 提供socket函数及数据结构
#include<netinet/in.h>  // 定义数据结构sockaddr_in

int make_socket (uint16_t port) {
    int sock; // socket 描述符
    sock = socket(AF_INET, SOCK_STREAM, 0); // IPv4 TCP socket
    if(sock < 0) {
        perror("socket fail"); // 相当于cerr
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ipv4_sock; // 定义IPv4 socket 对象

    ipv4_sock.sin_family = AF_INET;
    ipv4_sock.sin_port = htons(port);

    // sin_addr是in_addr类型的对象 
    // 设置通配地址
    ipv4_sock.sin_addr.s_addr = htonl(INADDR_ANY); 

    if(bind(sock, (struct sockaddr*) &ipv4_sock, sizeof(ipv4_sock)) < 0) { // 需要将socket格式转换为通用socket格式
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main() {
    int sockfd = make_socket(9819);
    printf("success!");
    exit(0);
}