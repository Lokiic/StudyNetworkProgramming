/*

实现功能：
1. 从连接套接字中获取字节流，并且判断出错和 EOF 情况
2. 如果对端发送“quit”就回应“COMAAND OK”的字符流

*/

#include "Common.h"

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
    
    char* response = "COMAAND OK";
    char recv_buf[BUFFER_SIZE];

    for (;;)
    {
        bzero(recv_buf, BUFFER_SIZE);
        // 注意：由于要在读取到的数据流后添加'\0',所以读取的数据量最多为缓冲区大小-1
        int rt = read(connectfd, recv_buf, BUFFER_SIZE - 1); 
        if (rt < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (rt == 0)
        {
            printf("client closed");
            exit(0);
        }

        recv_buf[rt] = '\0';
        if (strcmp(recv_buf, "quit") == 0)
        {
            printf("client quit");
            send(connectfd, response, strlen(response), 0);
        }
        
        printf("received %d bytes: %s \n", rt, recv_buf);
        
    }
    
}