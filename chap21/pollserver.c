/*

实现功能：
通过poll实现I/O多路复用，并检验该服务器程序的I/O复用能力

*/

#include "Common.h"

#define INIT_MAX 128

int main (int argc, char** argv) {
    int listenfd, connectfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        error(1, errno, "make socket fail");
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if(bind(listenfd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        error(1, errno, "bind fail");
    }

    if(listen(listenfd, 1024) < 0) {
        error(1, errno, "listen fail");
    }

    struct pollfd poll_set[INIT_MAX];
    poll_set[0].fd = listenfd;
    poll_set[0].events = POLLIN;

    int ready_number;
    // 初始化pollfd数组，元素为-1，表示当前未被占用
    int i;
    for(i = 1; i < INIT_MAX; ++i) {
        poll_set[i].fd = -1;
    }
    
    struct sockaddr_in client;
    char recv_buf[BUFFER_SIZE];
    for(;;) {
        if((ready_number = poll(poll_set, INIT_MAX, -1)) < 0) {
            error(1, errno, "poll fail");
        }

        // 有事件发生时，首先判断是不是有新的连接
        if(poll_set[0].revents & POLLIN) {
            socklen_t client_len = sizeof(client);
            connectfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
            if(connectfd < 0) {
                error(1, errno, "accept fail");
            }
            // 建立新的连接后，需要将connectfd放入pollfd数组中，之后就可以同时处理多个fd上的事件
            // 首先需要找到pollfd数组中的空位，然后放置
            for(i = 1; i < INIT_MAX; ++i) {
                if(poll_set[i].fd == -1) {
                    poll_set[i].fd = connectfd;
                    poll_set[i].events = POLLIN;
                    break;
                }
            }
            if(i == INIT_MAX) {
                error(1, errno, "pollfd array is full");
            }

            // 处理完一个文件描述符上的事件后，将总数量-1，当ready_number <= 0时，直接进入下次poll调用，加速程序
            if(--ready_number <= 0) 
                continue;
        }

        // 若 ready_number 仍然大于0，说明其他文件描述符上也有事件发生，那么需要遍历找到该描述符
        for(i = 1; i < INIT_MAX; ++i) {
            if(poll_set[i].fd == -1) {
                continue;
            }
            int sockfd = poll_set[i].fd;
            if(poll_set[i].revents & (POLLIN | POLLERR)) {
                printf("poll: read data on sockfd == %d \n", sockfd);
                // bzero(recv_buf, BUFFER_SIZE);
                // int rt = read(sockfd, recv_buf, BUFFER_SIZE);
                // if(rt > 0) {
                //     // 读到数据后，服务器端回显给客户端
                //     recv_buf[rt] = 0;
                //     int wt = write(sockfd, recv_buf, strlen(recv_buf));
                //     if(wt < 0) {
                //         error(1, errno, "write fail");
                //     }
                // }
                // else if(rt == 0 || errno == ECONNRESET) {
                //     printf("client closed");
                //     close(sockfd);
                //     poll_set[i].fd = -1;
                // }
                // else {
                //     error(1, errno, "read fail");
                // }
                
                // 处理完一个文件描述符后，将总量-1，并判断
                if(--ready_number <= 0)
                    break; // 跳出当前遍历pollfd数组的循环，直接进行下次poll调用
            }
        }
    }
}