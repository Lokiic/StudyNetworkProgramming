

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

    char buf[128];
    int rc;
    
    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        int n = strlen(buf);
        rc = send(sockfd, buf, n, 0);
        if (rc < 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }

        rc = read(sockfd, buf, sizeof(buf));
        if (rc < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (rc == 0)
        {
            printf("server quit");
            exit(0);
        }
        
        fputs(buf, stdout);
        
    }
    
}