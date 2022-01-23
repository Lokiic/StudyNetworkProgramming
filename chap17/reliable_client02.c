#include "Common.h"

void handler(int par)
{
    printf("SIGPIPE!");
    exit(0);
}

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

    signal(SIGPIPE, SIG_IGN);
    char* msg = "network programming";
    int count = 100000000;
    while (count > 0)
    {
        int wt = send(sockfd, msg, strlen(msg), 0);
        fprintf(stdout, "send into buffer %d \n", wt);
        if (wt < 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }

        --count;
        sleep(2);
    }
    
}