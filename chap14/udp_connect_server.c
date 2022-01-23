/*

实现功能：
1. 通过信号处理函数，响应"Ctrl + C" 操作
2. 通过recvfrom得到客户端信息，之后利用connect 将UDP socket和客户端 IPv4 地址绑定
3. 对客户端发送的信息加上"GO!"前缀，并发送给客户端
4. 不断地从客户端接收报文，该过程一直持续，直到客户端发送"goodbye"报文为止。

*/

#include "Common.h"

static int count;

void sighandler(int para)
{
    printf("received %d datagrams \n", count);
    exit(0);
}

int main (int argc, char** argv)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("make socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    count = 0;
    signal(SIGINT, sighandler);


    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    char recv_buf[BUFFER_SIZE];
    //bzero(recv_buf, BUFFER_SIZE);
    char send_line[MAXLINE];

    // 要先用recvfrom函数得到客户端地址信息，然后通过connect绑定
    int rt = recvfrom(sockfd, recv_buf, BUFFER_SIZE, 0, (struct sockaddr*) &client, &client_len);
    if (rt < 0)
    {
        perror("recv error");
        exit(EXIT_FAILURE);
    }
    printf("received %d bytes: %s \n", rt, recv_buf);
    ++count;

    if (connect(sockfd, (struct sockaddr*) &client, client_len) < 0)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    


    while (strncmp(recv_buf, "goodbye", 7) != 0) 
    {
        bzero(send_line, MAXLINE);
        sprintf(send_line, "GO! %s", recv_buf);

        size_t st = send(sockfd, send_line, strlen(send_line), 0);
        if (st < 0)
        {
            perror("send error");
            exit(EXIT_FAILURE);
        }
        printf("send %zu bytes \n", st);

        bzero(recv_buf, BUFFER_SIZE);
        size_t rc = recv(sockfd, recv_buf, BUFFER_SIZE, 0);
        if (rc < 0)
        {
            perror("recv error");
            exit(EXIT_FAILURE);
        }
        
        ++count;
        
    }
    
    
    
}