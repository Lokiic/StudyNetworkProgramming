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

    char recv_buf[1024];

    int time = 0;
    while (1)
    {
        int rt = read(connectfd, recv_buf, 1024);
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
        ++time;

        fprintf(stdout, "1K read for %d \n", time);
        usleep(10000);
        
    }
    
}