/*
实现功能：
1. 接收客户端发送的数据报
2. 对接收到的数据报加上前缀 "GO!" 并发回给客户端
*/

#include"Common.h"

// 数据报形式的本地socket 类似 UDP socket
int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("path error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    char* localpath = argv[1];
    unlink(localpath);

    struct sockaddr_un ser_addr, cli_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, localpath);
    
    if (bind(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }
    
    char recv_buf[BUFFER_SIZE];
    socklen_t cli_len = sizeof(cli_addr);

    for (;;)
    {
        bzero(recv_buf, BUFFER_SIZE);
        if (recvfrom(sockfd, recv_buf, BUFFER_SIZE, 0, (struct sockaddr*) &cli_addr, &cli_len) == 0)
        {
            printf("client quit");
            break;
        }
        printf("Receive: %s \n", recv_buf);

        char send_line[MAXLINE];
        bzero(send_line, MAXLINE);

        sprintf(send_line, "GO! %s", recv_buf);

        size_t n = strlen(send_line);
        printf("Now sending %s \n", send_line);

        if (sendto(sockfd, send_line, n, 0, (struct sockaddr*)& cli_addr, cli_len) != n)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }
        
        
    }
    
    close(sockfd);

    exit(0);
}