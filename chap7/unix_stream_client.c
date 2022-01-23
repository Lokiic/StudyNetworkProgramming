/*

实现功能：
1. 通过标准输入读取数据，并将该数据发给服务器端
2. 将从服务器端接收到数据，打印在标准输出上

*/

#include"Common.h"

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("path error");
        exit(EXIT_FAILURE);
    }
    
    int sockfd;
    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, argv[1]);

    if (connect(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    
    char send_line[MAXLINE];
    bzero(send_line, sizeof(send_line));
    char recv_buf[BUFFER_SIZE];

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int n = sizeof(send_line);
        if (write(sockfd, send_line, n) != n)
        {
            perror("write error");
            exit(EXIT_FAILURE);
        }
        
        if (read(sockfd, recv_buf, BUFFER_SIZE) == 0)
        {
            perror("server is close");
            exit(EXIT_FAILURE);
        }
        fputs(recv_buf, stdout);
        
    }
    
    exit(0);

}