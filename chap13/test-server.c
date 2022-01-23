/*

实现功能：
打印从客户端收到的数据，以及字节数

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

    char read_buf[BUFFER_SIZE];
    for (;;)
    {
        bzero(read_buf, BUFFER_SIZE);
        int n = read(connectfd, read_buf, BUFFER_SIZE);
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            perror("client quit");
            exit(EXIT_FAILURE);
        }

        printf("received %d bytes: %s \n", n, read_buf);
        
    }
    

}