/*

实现功能：
报文分界符是'\n'
1. 如果不是报文分界符，则将数据拷贝到缓冲区
2. 如果是报文分界符，则将缓冲区截断，返回最终读取的字符个数

*/

#include "Common.h"

size_t readline(int fd, char* buf, size_t length)
{
    char* buf_first = buf; // 记录缓冲区一开始的位置，方便计算读入字符的个数
    char* buf_temp;
    char temp[512];
    char c;
    int sum = 0;
    while (--length > 0)
    {
        if (sum <= 0)
        {
            int nread = recv(fd, temp, sizeof(temp), 0);
            if (nread < 0)
            {
                if (errno == EINTR)
                {
                    ++length;
                    continue;
                }
                else
                    return -1;
            }
            else if (nread == 0)
                return 0;
            
            sum = nread;
            buf_temp = temp;
        }
        // 读取临时缓冲区的字符，将其拷贝到要存储的缓冲区中
        c = *buf_temp;
        ++buf_temp;

        *buf = c;
        ++buf;
        --sum;
        if (c == '\n') // 如果是分界符，需要将缓冲区隔断，并返回最终读取的字符个数
        {
            *buf = '\0';
            return buf - buf_first;
        }
        
    }
    return -1; // 消息过长，应报错，消息最后应该以'\n'结尾，并在缓冲区中加入'\0'
}




int main (int argc, char** argv)
{
    int listenfd, connectfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (bind(listenfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 1024) < 0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    connectfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }

    char recv_buf[10];

    while (1)
    {
        bzero(recv_buf, 10);
        int n = readline(connectfd, recv_buf, 10);
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            printf("client closed");
            exit(0);
        }
        printf("received %d bytes: %s \n", n, recv_buf);
        
    }
    
}    