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

    connectfd = accept(listenfd, (struct sockadrr*) &client, &client_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }
    
    char recv_buf[BUFFER_SIZE];
    for (;;)
    {
        bzero(recv_buf, BUFFER_SIZE);
        int rt = read(connectfd, recv_buf, BUFFER_SIZE);
        if (rt < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (rt == 0)
        {
            printf("client quit");
            exit(0);
        }

        printf("received %d bytes: %s \n", rt, recv_buf);
        
    }
    
    
}