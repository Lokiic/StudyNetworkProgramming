/*

实现功能：
通过select实现I/O多路复用

*/

#include "Common.h"

int main (int argc, char** argv) {
    if(argc != 2) {
        perror("ip error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if(connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }

    char recv_buf[BUFFER_SIZE];
    char send_line[MAXLINE];

    fd_set allreads;
    fd_set readmask;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(sockfd, &allreads);

    for(;;) {
        readmask = allreads;
        if(select(sockfd + 1, &readmask, NULL, NULL, NULL) < 0) {
            perror("select fail");
            exit(EXIT_FAILURE);
        }
        if(FD_ISSET(sockfd, &readmask)) {
            printf("read data on sockfd == %d \n", sockfd);
            // bzero(recv_buf, BUFFER_SIZE);
            // int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            // if(rt <  0) {
            //     perror("read fail");
            //     exit(EXIT_FAILURE);
            // }
            // else if(rt == 0) {
            //     printf("server closed");
            //     exit(0);
            // }
            // recv_buf[rt] = 0;
            // fputs(recv_buf, stdout);
            // fputs("\n", stdout);
        }

        if(FD_ISSET(0, &readmask)) {
            if(fgets(send_line, MAXLINE, stdin) != NULL) {
                int st = strlen(send_line);
                if(send_line[st - 1] == '\n')
                    send_line[st - 1] = 0;
                printf("now sending %s \n", send_line);
                if(send(sockfd, send_line, strlen(send_line), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }


}