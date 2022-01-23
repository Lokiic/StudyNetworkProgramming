/*

ʵ�ֹ��ܣ�
ͨ��pollʵ��I/O��·���ã�������÷����������I/O��������

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
    // ��ʼ��pollfd���飬Ԫ��Ϊ-1����ʾ��ǰδ��ռ��
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

        // ���¼�����ʱ�������ж��ǲ������µ�����
        if(poll_set[0].revents & POLLIN) {
            socklen_t client_len = sizeof(client);
            connectfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
            if(connectfd < 0) {
                error(1, errno, "accept fail");
            }
            // �����µ����Ӻ���Ҫ��connectfd����pollfd�����У�֮��Ϳ���ͬʱ������fd�ϵ��¼�
            // ������Ҫ�ҵ�pollfd�����еĿ�λ��Ȼ�����
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

            // ������һ���ļ��������ϵ��¼��󣬽�������-1����ready_number <= 0ʱ��ֱ�ӽ����´�poll���ã����ٳ���
            if(--ready_number <= 0) 
                continue;
        }

        // �� ready_number ��Ȼ����0��˵�������ļ���������Ҳ���¼���������ô��Ҫ�����ҵ���������
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
                //     // �������ݺ󣬷������˻��Ը��ͻ���
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
                
                // ������һ���ļ��������󣬽�����-1�����ж�
                if(--ready_number <= 0)
                    break; // ������ǰ����pollfd�����ѭ����ֱ�ӽ����´�poll����
            }
        }
    }
}