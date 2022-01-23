/*

实现功能：
1. 可以指定待连接的服务器端 IP 地址和端口。
2. 在输入一个命令之后，回车结束，之后等待服务器端将执行结果返回，客户端程序需要将结果显示在屏幕上。
3. 如果输入的是quit，那么客户端退出

*/

#include "Common.h"

// 需要指定ip地址和端口，所以输入参数的数量应该为3
int main (int argc, char** argv) {
    if (argc != 3) {
        perror("error input");
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
    int port = atoi(argv[2]);
    server.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    

    char recv_buf[BUFFER_SIZE];
    char send_line[MAXLINE];

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(sockfd, &allreads);

    for (;;) {
        //  因为每次select操作完就会把传入的fd_set修改掉，
        //  以便告知应用程序什么样的事件发生了，
        //  所以需要一个初始化的fd_set保存感兴趣的待检测fd_set。
        readmask = allreads; 
        // 给最后一个参数timeout传递NULL，select将一直阻塞，直到某个文件描述符就绪
        int n = select(sockfd + 1, &readmask, NULL, NULL, NULL); 
        if (n < 0) {
            perror("select fail");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(sockfd, &readmask)) {
            bzero(recv_buf, BUFFER_SIZE);
            int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            if (rt < 0){
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if (rt == 0) {
                printf("server closed");
                exit(0);
            }
            recv_buf[rt] = 0;
            fputs(recv_buf, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(0, &readmask)) {
            bzero(send_line, MAXLINE);
            if (fgets(send_line, MAXLINE, stdin) != NULL){
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n')
                    send_line[i - 1] = 0;
                
                if (strncmp(send_line, "quit", strlen("quit")) == 0) {
                    if (shutdown(sockfd, SHUT_WR) == -1) {
                        perror("shutdown fail");
                        exit(EXIT_FAILURE);
                    }
                }

                int wt = write(sockfd, send_line, strlen(send_line));
                if (wt < 0)
                {
                    perror("write fail");
                    exit(EXIT_FAILURE);
                }
            }
            
        }
        
    }
    
}