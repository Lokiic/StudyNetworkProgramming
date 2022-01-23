/*

ʵ�ֹ���:
1. ��ÿ���������ӽ����󣬴���һ�����߳������������
2. ���߳���ͨ��ʵ�� echo ���������ͻ��˵����ݱ��벢���ͻؿͻ���

*/

#include "Common.h"

extern void echo(int fd);

void* thread_run(void* arg) {
    pthread_detach(pthread_self()); // �����߳�����Ϊ�����
    int fd = (int) arg;
    echo(fd);
}

int main(int argc, char** argv) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        error(1, errno, "make socket fail");
    }

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        error(1, errno, "bind fail");
    }

    if(listen(listenfd, 1024) < 0) {
        error(1, errno, "listen fail");
    }

    
    while(1) {
        pthread_t tid;
        struct sockaddr_storage client;
        bzero(&client, sizeof(client));
        socklen_t clientLen = sizeof(client);
        int fd = accept(listenfd, (struct sockaddr*) &client, &clientLen);
        if(fd < 0) {
            error(1, errno, "accept fail");
        }
        else {
            pthread_create(&tid, NULL, &thread_run, (void*) fd);
        }
    }
}
