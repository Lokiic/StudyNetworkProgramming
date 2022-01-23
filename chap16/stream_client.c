/*

实现功能：
1. 建立消息结构体
2. 从标准输入读入数据，初始化结构体

*/

#include "Common.h"

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }


    struct
    {
        u_int32_t length;
        u_int32_t type;
        char buf[128];
    }message;  // message是一个变量

    while (fgets(message.buf, sizeof(message.buf), stdin) != NULL)
    {
        int n = strlen(message.buf);
        message.length = htons(n);
        message.type = htons(1);
        if (send(sockfd, (char *) &message, n + sizeof(message.length) + sizeof(message.type), 0) < 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }
        
    }
    
    
    
}