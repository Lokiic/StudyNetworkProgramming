/*

实现功能：
1. 基于fork实现 阻塞I/O + 多进程模型
2. 对于客户端发送的数据，由子进程进行ROT13处理，并发回给客户端
3. 捕获SIGCHLD信号，及时回收子进程资源

*/

#include "Common.h"

char ROT13(char c) {
    if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    return c;
}

void handle_child(int sig) {
    // while循环处理，保证所有结束的子进程都被回收
    while(waitpid(-1, 0, WNOHANG) > 0) {}
    // waitpid返回0时，表示还没有子进程退出
    return;
}

void child_run(int fd) {
    printf("child run");
    char recv_buf[BUFFER_SIZE];
    bzero(recv_buf, BUFFER_SIZE);
    size_t index = 0;
    while(1) {
        char ch;
        int rt = recv(fd, &ch, 1, 0);
        if(rt == 0) {
            break;
        }
        else if(rt < 0) {
            perror("recv fail");
            break;
        }


        if(ch == '\n') {
            if(write(fd, recv_buf, index) < 0) {
                perror("write fail");
                break;
            }
            index = 0;
            continue;
        }

        if(index < sizeof(recv_buf)) {
            recv_buf[index++] = ROT13(ch);
        }


    }
}

int main(int argc, char** argv) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        error(1, errno, "make socket fail");
    }

    /* 实现阻塞I/O + 多进程模型，所以不需要将listenfd设置为非阻塞 */

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

    signal(SIGCHLD, handle_child);

    while(1) {
        struct sockaddr_storage client;
        socklen_t client_len = sizeof(client);
        int connect_fd;
        connect_fd = accept(listenfd, (struct sockaddr*) &client, &client_len);
        if(connect_fd < 0) {
            error(1, errno, "accept fail");
        }

        if(fork() == 0) {
            // 子进程中只处理connect_fd，所以先close listenfd
            close(listenfd);
            child_run(connect_fd);
            exit(0);
        }
        else {
            // 父进程中只处理listenfd，所以close connect_fd
            close(connect_fd);
        }
    }

    close(listenfd);
    exit(0);
}