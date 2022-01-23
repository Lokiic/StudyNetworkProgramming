/*

此客户端程序仅用来测试服务器程序

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

    // while (fgets(send_line, MAXLINE, stdin) != NULL)
    // {
    //     bzero(recv_buf, BUFFER_SIZE);
    //     int n = strlen(send_line);
    //     if (send_line[n - 1] == '\n')
    //         send_line[n - 1] = 0;
        
    //     size_t st = send(sockfd, send_line, strlen(send_line), 0);
    //     if (st < 0)
    //     {
    //         perror("send fail");
    //         exit(EXIT_FAILURE);
    //     }
    //     printf("now sending %zu bytes: %s \n", st, send_line);

    //     size_t rt = read(sockfd, recv_buf, BUFFER_SIZE);
    //     if (rt < 0)
    //     {
    //         perror("read fail");
    //         exit(EXIT_FAILURE);
    //     }
    //     else if (rt == 0)
    //     {
    //         printf("connection closed by server");
    //         exit(0);
    //     }

    //     recv_buf[rt] = 0;
    //     fputs(recv_buf, stdout);
    //     fputs("\n", stdout);

    //     bzero(send_line, MAXLINE);
        
    // }

    fd_set allread;
    fd_set readmask;
    FD_ZERO(&allread);
    FD_ZERO(&readmask);
    FD_SET(0, &allread);
    FD_SET(sockfd, &allread);

    for(;;) {
        readmask = allread;
        if(select(sockfd + 1, &readmask, NULL, NULL, NULL) < 0) {
            error(1, errno, "select fail");
        }

        if(FD_ISSET(sockfd, &readmask)) {
            bzero(recv, BUFFER_SIZE);
            int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            if(rt < 0) {
                error(1, errno, "read fail");
            }
            else if(rt == 0) {
                printf("server closed");
                exit(0);
            }
            else {
                recv_buf[rt] = 0;
                fputs(recv_buf, stdout);
                fputs("\n", stdout);
            }
        }

        if(FD_ISSET(0, &readmask)) {
            bzero(send_line, MAXLINE);
            if(fgets(send_line, MAXLINE, stdin) != NULL) {
                int st = strlen(send_line);
                if(send_line[st - 1] == '\n')
                    send_line[st - 1] = 0;
                
                printf("now sending %d bytes: %s \n", st, send_line);
                
                if(write(sockfd, send_line, strlen(send_line)) < 0) {
                    error(1, errno, "write fail");
                }
            }
        }
    }
}