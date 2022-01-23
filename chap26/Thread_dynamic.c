/*

实现功能:
1. 在每次有新连接建立后，创建一个新线程来处理该连接
2. 子线程中通过实现 echo 函数，将客户端的数据编码并发送回客户端

*/

#include "Common.h"

extern void echo(int fd);

void* thread_run(void* arg) {
    pthread_detach(pthread_self()); // 将子线程设置为分离的
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
