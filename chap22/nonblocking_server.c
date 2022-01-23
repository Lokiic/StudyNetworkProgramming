/*

实现功能：
1. 结合非阻塞I/O 和 select I/O多路复用
2. 建立应用层缓冲区
3. 设计ROT13函数，对客户端的数据进行置换，并发回给客户端
4. 设计onSocketRead函数通过套接字读取数据，数据存放在Buffer对象里，Buffer对象通过了writeIndex记录当前数据区可写的位置
5. 设计onSocketWrite函数通过套接字写数据，数据来源于Buffer缓冲对象，Buffer缓冲对象的readIndex记录了当前缓冲区读的位置

*/

#include "Common.h"
#define INIT_SIZE 128

// 应用层缓冲区设计
struct Buffer {
    int connect_fd; // 连接字
    char buf[1024]; // 缓冲区
    size_t readIndex;  // 读的位置
    size_t writeIndex;  // 写的位置
    int readable;   // 是否可以读（当读完所有数据时，修改该标志，表示目前缓冲区内的数据可以由应用程序读取）
};

// 初始化Buffer对象
struct Buffer* malloc_Buffer() {
    struct Buffer* buffer = malloc(sizeof(struct Buffer));
    if(!buffer) {  // 调用malloc返回空时，此函数也返回空
        return NULL;
    }
    buffer->connect_fd = 0;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
    buffer->readable = 0;
    return buffer;
}

// 释放Buffer对象
void freeBuffer(struct Buffer* buffer) {
    free(buffer);
}

// ROT13函数设计，实现数据的置换编码
char ROT_13(char c) {
    if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}

/*

onSocketRead函数
通过套接字读取数据，数据存放在Buffer对象里，Buffer对象通过了writeIndex记录当前数据区可写的位置
从fd套接字读取数据，数据先读取到本地buf数组中，再逐个字节拷贝到buffer对象缓冲中

*/
int onSocketRead(int fd, struct Buffer* buffer) {
    char recv_buf[1024];
    ssize_t result;  // 这里需要是有符号型变量，而不能是无符号型 size_t！！！！
    int i;
    while(1) {
        result = recv(fd, recv_buf, 1024, 0);
        if(result <= 0) // result<=0时，跳出循环，在循环外对result进行判断
            break;
        for(i = 0; i < result; ++i) {
            if(buffer->writeIndex < sizeof(buffer->buf))
                buffer->buf[buffer->writeIndex++] = ROT_13(recv_buf[i]);
            //如果读取了回车符，则认为client端发送结束，此时可以把编码后的数据回送给客户端
            if(recv_buf[i] == '\n')
                buffer->readable = 1;
        }
    }

    if(result == 0) {
        return 1; // recv返回0时，说明对端已经关闭，此时本函数返回1，通过此函数的返回值可以判断是否close套接字
    }
    else if(result < 0) {
        if(errno == EAGAIN)
            return 0;
        else
            return -1;
    }

    return 0;
}

/*

从buffer对象的readIndex开始读，一直读到writeIndex的位置，这段区间是有效数据
如果readindex已经追上writeIndex，说明有效发送区间已经全部读完，将readIndex和writeIndex设置为0，复用这段缓冲
缓冲数据已经全部读完，不需要再读（将readable设置为0）

*/
int onSocketWrite(int fd, struct Buffer* buffer) {
    while(buffer->readIndex < buffer->writeIndex) {
        // 这里需要是有符号型变量，而不能是无符号型 size_t！！！！
        ssize_t st = send(fd, buffer->buf + buffer->readIndex, buffer->writeIndex - buffer->readIndex, 0);
        if(st < 0) {
            if(errno == EAGAIN)
                return 0;
            return -1;
        }

        buffer->readIndex += st;
    }

    if(buffer->readIndex == buffer->writeIndex) {
        buffer->readIndex = 0;
        buffer->writeIndex = 0;
    }

    buffer->readable = 0;

    return 0;
}

int main (int argc, char** argv) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        error(1, errno, "make socket fail");
    }

    // 将listenfd设置为非阻塞模式
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

    signal(SIGPIPE, SIG_IGN);

    // 建立一个Buffer指针数组，为每个指针分配一个初始化对象
    struct Buffer* buffer_array[INIT_SIZE];
    for(int i = 0; i < INIT_SIZE; ++i) {
        buffer_array[i] = malloc_Buffer();
    }

    fd_set readset, writeset, exset;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exset);

    int maxfd; // 用于记录最大的文件描述符fd
    int i;
    while(1) {
        maxfd = listenfd;

        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exset);

        // 将listenfd加入readset
        FD_SET(listenfd, &readset);

        // 遍历Buffer指针数组，将存在的文件描述符加入readset
        // 如果某个元素可读，说明其数据已经读取完毕，那么将其加入writeset
        for(i = 0; i < INIT_SIZE; ++i) {
            if(buffer_array[i]->connect_fd > 0) {
                if(buffer_array[i]->connect_fd > maxfd) {
                    maxfd = buffer_array[i]->connect_fd;
                }
                FD_SET(buffer_array[i]->connect_fd, &readset);
                if(buffer_array[i]->readable) {
                    FD_SET(buffer_array[i]->connect_fd, &writeset);
                }
            }
        }

        // select I/O多路复用
        if(select(maxfd + 1, &readset, &writeset, &exset, NULL) < 0) {
            error(1, errno, "select fail");
        }

        if(FD_ISSET(listenfd, &readset)) {
            printf("listening socket readable \n");
            //sleep(5); // 可以用于实验：accept是否会阻塞
            //printf("no error 0");
            struct sockaddr_storage client;
            socklen_t client_len = sizeof(client);
            int sockfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
            if(sockfd < 0) {
                error(1, errno, "accept fail");
            }
            else if(sockfd >= INIT_SIZE) { // 当sockfd超过了数组长度时，说明连接以达上限
                //printf("sockfd >= INIT_SIZE");
                error(1, 0, "too many connections");
                close(sockfd);
            }
            else{
                fcntl(sockfd, F_SETFL, O_NONBLOCK); // 将sockfd设置为非阻塞
                printf("no error 1");
                if(buffer_array[sockfd]->connect_fd == 0) {
                    buffer_array[sockfd]->connect_fd = sockfd;
                }
                else{
                    error(1, 0, "too many connections");
                }
            }

        }

        // 对listenfd判断完后，需要对buffer数组中的其他连接字进行判断
        for(i = 0; i <= maxfd; ++i) {
            //printf("no error 2");
            if(i == listenfd)
                continue;
            int r = 0; // 对onSocketRead和onSocketWrite函数的返回值进行判断
            /*
            
            onSocketRead 
            当recv返回0时，该函数返回1
            当recv返回-1，且errno == EAGAIN，该函数返回0 否则该函数返回-1
            除以上两种情况外，该函数返回0

            */
            if(FD_ISSET(i, &readset)) {
                r = onSocketRead(i, buffer_array[i]);
            }

            if(r == 0 && FD_ISSET(i, &writeset)) {
                //r = onSocketWrite(i, buffer_array[i]);
                while(buffer_array[i]->readIndex < buffer_array[i]->writeIndex) {
                    // 这里需要是有符号型变量，而不能是无符号型 size_t！！！！
                    ssize_t st = send(i, buffer_array[i]->buf + buffer_array[i]->readIndex, buffer_array[i]->writeIndex - buffer_array[i]->readIndex, 0);
                    if(st < 0) {
                        if(errno == EAGAIN)
                            return 0;
                        return -1;
                    }

                    buffer_array[i]->readIndex += st;
                }

                if(buffer_array[i]->readIndex == buffer_array[i]->writeIndex) {
                    buffer_array[i]->readIndex = 0;
                    buffer_array[i]->writeIndex = 0;
                }

                buffer_array[i]->readable = 0;
            }

            // 当 r==1时，说明对端关闭了连接，此时应该调用close，并处理buffer数组
            if(r) {
                buffer_array[i]->connect_fd = 0;
                close(i);
            }
        }
    }

    for(int i = 0; i < INIT_SIZE; ++i) {
        freeBuffer(buffer_array[i]);
    }
}