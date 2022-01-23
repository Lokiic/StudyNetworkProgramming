/*

epoll2_server.c �� epoll3_server.c �������� ET �� LT ģʽ������

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

    events = (struct epoll_event*)calloc(MaxEvents, sizeof(event)); // Ϊevents��������ڴ�ռ�

    for(;;) {
        int n = epoll_wait(epfd, events, MaxEvents, -1);
        if(n < 0) {
            error(1, errno, "epoll wait fail");
        }
        printf("call epoll_wait \n");
        
        // ��epoll_wait���صľ����¼�����һ�ж�
        for(int i = 0; i < n; ++i) {
            // 1.������쳣�¼��Ļ���Ӧ�ùرմ��ļ�������
            if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                fprintf(stderr, "epoll error");
                close(events[i].data.fd);
                continue;
            }
            // 2.�����쳣�¼��Ļ����ж��Ƿ���listenfd������ǣ���ô��Ҫ����accept��������
            else if(events[i].data.fd == listenfd) {
                struct sockaddr_storage client;
                socklen_t client_len = sizeof(client);
                int fd = accept(listenfd, (struct sockaddr*) &client, &client_len);
                if(fd < 0) {
                    error(1, errno, "accept fail");
                }
                // ��connect_fd����Ϊ������
                fcntl(fd, F_SETFL, O_NONBLOCK);
                // ���õ���connect_fd����epollʵ��
                event.events = EPOLLIN;   // ����ʹ��Ĭ�ϵ� LT ģʽ
                event.data.fd = fd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) < 0) {
                    error(1, errno, "epoll ctl fail");
                }
                continue;
            }
            // 3.�������ļ������������¼�����ʱ��Ӧ������������¼�����ȡ�ͻ������ݣ������ԣ�
            else {
                int sockfd = events[i].data.fd;
                printf("get event on sockfd == %d \n", sockfd);
                // char recv_buf[BUFFER_SIZE];
                // // ���ڲ���ETģʽ�������¼����ᱻ�ظ�����������Ӧ����ѭ��ȷ��socket�������е�����ȫ������
                // while(1) {
                //     int rt = recv(sockfd, recv_buf, BUFFER_SIZE, 0);
                //     if(rt < 0) {
                //         // ����sockfd�Ƿ������ģ�������Ҫ�жϴ�����Ϣ�ǲ���EAGAIN��������ǵĻ�����Ҫ�ر�sockfd
                //         if(errno != EAGAIN) {
                //             error(1, errno, "recv fail");
                //             close(sockfd);
                //         }
                //         // �����EAGAIN�Ļ�����ʾ�����Ѿ�ȫ����ȡ��ϣ���ʱ����ѭ������������epoll
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