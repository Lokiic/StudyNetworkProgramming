/*

用于对服务器端进行测试

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
    char recv_buf[BUFFER_SIZE];
    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int n = strlen(send_line);
        // if (send_line[n - 1] == '\n')
        //     send_line[n - 1] = 0;
        int st = send(sockfd, send_line, strlen(send_line), 0);
        if (st < 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }
        if (strcmp(send_line, "quit") == 0)
        {
            int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            if (rt < 0)
            {
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if(rt == 0)
            {
                printf("server closed");
                exit(0);
            }
            printf("receive: %s", recv_buf);
            exit(0);
            
        }
        
    }
    
}