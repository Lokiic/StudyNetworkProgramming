/*

实现功能：
通过writev函数，把几个内存块中的数据合并成一个有序的字节流

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

    char send_line[MAXLINE];
    struct iovec iov[2];
    
    char *str = "hello";
    iov[0].iov_base = str;
    iov[0].iov_len = strlen(str);
    iov[1].iov_base = send_line;
    
    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int len = strlen(send_line);
        if(send_line[len - 1] == '\n')
            send_line[len - 1] = 0;
        iov[1].iov_len = strlen(send_line);

        int wt = writev(sockfd, iov, 2);
        if(wt < 0)
        {
            perror("writev fail");
            exit(EXIT_FAILURE);
        }
        //bzero(send_line, MAXLINE);
    }
    
}