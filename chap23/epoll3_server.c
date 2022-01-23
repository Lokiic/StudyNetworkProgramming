/*

epoll2_server.c 和 epoll3_server.c 用来测试 ET 和 LT 模式的区别

*/

#include "Common.h"
#define MaxEvents 128

char ROT13(char c) {
    if(c >= 'a' && c <= 'm' || c >= 'A' && c <= 'M')
        return c + 13;
    else if(c >= 'n' && c <= 'z' || c >= 'N' && c <= 'Z')
        return c - 13;
    else
        return c;
}

int main(int argc, char** argv) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        error(1, errno, "make socket fail");
    }

    fcntl(listenfd, F_SETFL, O_NONBLOCK);

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

    struct epoll_event event;
    struct epoll_event* events;

    int epfd;
    epfd = epoll_create1(0);
    if(epfd < 0) {
        error(1, errno, "epoll create fail");
    }

    event.events = EPOLLIN;
    event.data.fd = listenfd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event) < 0) {
        error(1, errno, "epoll ctl fail");
    }

    events = (struct epoll_event*)calloc(MaxEvents, sizeof(event)); // 为events数组分配内存空间

    for(;;) {
        int n = epoll_wait(epfd, events, MaxEvents, -1);
        if(n < 0) {
            error(1, errno, "epoll wait fail");
        }
        printf("call epoll_wait \n");
        
        // 对epoll_wait返回的就绪事件集逐一判断
        for(int i = 0; i < n; ++i) {
            // 1.如果是异常事件的话，应该关闭此文件描述符
            if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                fprintf(stderr, "epoll error");
                close(events[i].data.fd);
                continue;
            }
            // 2.不是异常事件的话，判断是否是listenfd，如果是，那么需要调用accept建立连接
            else if(events[i].data.fd == listenfd) {
                struct sockaddr_storage client;
                socklen_t client_len = sizeof(client);
                int fd = accept(listenfd, (struct sockaddr*) &client, &client_len);
                if(fd < 0) {
                    error(1, errno, "accept fail");
                }
                // 将connect_fd设置为非阻塞
                fcntl(fd, F_SETFL, O_NONBLOCK);
                // 将得到的connect_fd加入epoll实例
                event.events = EPOLLIN;   // 这里使用默认的 LT 模式
                event.data.fd = fd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) < 0) {
                    error(1, errno, "epoll ctl fail");
                }
                continue;
            }
            // 3.是其他文件描述符上有事件发生时，应该立即处理该事件（读取客户端数据，并回显）
            else {
                int sockfd = events[i].data.fd;
                printf("get event on sockfd == %d \n", sockfd);
                // char recv_buf[BUFFER_SIZE];
                // // 由于采用ET模式，所以事件不会被重复触发，所以应该用循环确保socket读缓存中的数据全部读出
                // while(1) {
                //     int rt = recv(sockfd, recv_buf, BUFFER_SIZE, 0);
                //     if(rt < 0) {
                //         // 由于sockfd是非阻塞的，所以需要判断错误信息是不是EAGAIN，如果不是的话，需要关闭sockfd
                //         if(errno != EAGAIN) {
                //             error(1, errno, "recv fail");
                //             close(sockfd);
                //         }
                //         // 如果是EAGAIN的话，表示数据已经全部读取完毕，此时跳出循环，继续调用epoll
                //         printf("all data is read \n");
                //         break;
                //     }
                //     else if(rt == 0) {
                //         printf("client closed");
                //         close(sockfd);
                //         break;
                //     }
                //     else {
                //         for(int i = 0; i < rt; ++i) {
                //             recv_buf[i] = ROT13(recv_buf[i]);
                //         }
                //         if(write(sockfd, recv_buf, strlen(recv_buf)) < 0) {
                //             error(1, errno, "write fail");
                //         }
                //     }
                // }
            }
        }
    }
    free(events);
    close(epfd);
    exit(0);

}