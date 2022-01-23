/*

ʵ�ֹ��ܣ�
1. ��Ϸ�����I/O �� select I/O��·����
2. ����Ӧ�ò㻺����
3. ���ROT13�������Կͻ��˵����ݽ����û��������ظ��ͻ���
4. ���onSocketRead����ͨ���׽��ֶ�ȡ���ݣ����ݴ����Buffer�����Buffer����ͨ����writeIndex��¼��ǰ��������д��λ��
5. ���onSocketWrite����ͨ���׽���д���ݣ�������Դ��Buffer�������Buffer��������readIndex��¼�˵�ǰ����������λ��

*/

#include "Common.h"
#define INIT_SIZE 128

// Ӧ�ò㻺�������
struct Buffer {
    int connect_fd; // ������
    char buf[1024]; // ������
    size_t readIndex;  // ����λ��
    size_t writeIndex;  // д��λ��
    int readable;   // �Ƿ���Զ�����������������ʱ���޸ĸñ�־����ʾĿǰ�������ڵ����ݿ�����Ӧ�ó����ȡ��
};

// ��ʼ��Buffer����
struct Buffer* malloc_Buffer() {
    struct Buffer* buffer = malloc(sizeof(struct Buffer));
    if(!buffer) {  // ����malloc���ؿ�ʱ���˺���Ҳ���ؿ�
        return NULL;
    }
    buffer->connect_fd = 0;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
    buffer->readable = 0;
    return buffer;
}

// �ͷ�Buffer����
void freeBuffer(struct Buffer* buffer) {
    free(buffer);
}

// ROT13������ƣ�ʵ�����ݵ��û�����
char ROT_13(char c) {
    if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}

/*

onSocketRead����
ͨ���׽��ֶ�ȡ���ݣ����ݴ����Buffer�����Buffer����ͨ����writeIndex��¼��ǰ��������д��λ��
��fd�׽��ֶ�ȡ���ݣ������ȶ�ȡ������buf�����У�������ֽڿ�����buffer���󻺳���

*/
int onSocketRead(int fd, struct Buffer* buffer) {
    char recv_buf[1024];
    ssize_t result;  // ������Ҫ���з����ͱ��������������޷����� size_t��������
    int i;
    while(1) {
        result = recv(fd, recv_buf, 1024, 0);
        if(result <= 0) // result<=0ʱ������ѭ������ѭ�����result�����ж�
            break;
        for(i = 0; i < result; ++i) {
            if(buffer->writeIndex < sizeof(buffer->buf))
                buffer->buf[buffer->writeIndex++] = ROT_13(recv_buf[i]);
            //�����ȡ�˻س���������Ϊclient�˷��ͽ�������ʱ���԰ѱ��������ݻ��͸��ͻ���
            if(recv_buf[i] == '\n')
                buffer->readable = 1;
        }
    }

    if(result == 0) {
        return 1; // recv����0ʱ��˵���Զ��Ѿ��رգ���ʱ����������1��ͨ���˺����ķ���ֵ�����ж��Ƿ�close�׽���
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

��buffer�����readIndex��ʼ����һֱ����writeIndex��λ�ã������������Ч����
���readindex�Ѿ�׷��writeIndex��˵����Ч���������Ѿ�ȫ�����꣬��readIndex��writeIndex����Ϊ0��������λ���
���������Ѿ�ȫ�����꣬����Ҫ�ٶ�����readable����Ϊ0��

*/
int onSocketWrite(int fd, struct Buffer* buffer) {
    while(buffer->readIndex < buffer->writeIndex) {
        // ������Ҫ���з����ͱ��������������޷����� size_t��������
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

    // ��listenfd����Ϊ������ģʽ
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

    // ����һ��Bufferָ�����飬Ϊÿ��ָ�����һ����ʼ������
    struct Buffer* buffer_array[INIT_SIZE];
    for(int i = 0; i < INIT_SIZE; ++i) {
        buffer_array[i] = malloc_Buffer();
    }

    fd_set readset, writeset, exset;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exset);

    int maxfd; // ���ڼ�¼�����ļ�������fd
    int i;
    while(1) {
        maxfd = listenfd;

        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exset);

        // ��listenfd����readset
        FD_SET(listenfd, &readset);

        // ����Bufferָ�����飬�����ڵ��ļ�����������readset
        // ���ĳ��Ԫ�ؿɶ���˵���������Ѿ���ȡ��ϣ���ô�������writeset
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

        // select I/O��·����
        if(select(maxfd + 1, &readset, &writeset, &exset, NULL) < 0) {
            error(1, errno, "select fail");
        }

        if(FD_ISSET(listenfd, &readset)) {
            printf("listening socket readable \n");
            //sleep(5); // ��������ʵ�飺accept�Ƿ������
            //printf("no error 0");
            struct sockaddr_storage client;
            socklen_t client_len = sizeof(client);
            int sockfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
            if(sockfd < 0) {
                error(1, errno, "accept fail");
            }
            else if(sockfd >= INIT_SIZE) { // ��sockfd���������鳤��ʱ��˵�������Դ�����
                //printf("sockfd >= INIT_SIZE");
                error(1, 0, "too many connections");
                close(sockfd);
            }
            else{
                fcntl(sockfd, F_SETFL, O_NONBLOCK); // ��sockfd����Ϊ������
                printf("no error 1");
                if(buffer_array[sockfd]->connect_fd == 0) {
                    buffer_array[sockfd]->connect_fd = sockfd;
                }
                else{
                    error(1, 0, "too many connections");
                }
            }

        }

        // ��listenfd�ж������Ҫ��buffer�����е����������ֽ����ж�
        for(i = 0; i <= maxfd; ++i) {
            //printf("no error 2");
            if(i == listenfd)
                continue;
            int r = 0; // ��onSocketRead��onSocketWrite�����ķ���ֵ�����ж�
            /*
            
            onSocketRead 
            ��recv����0ʱ���ú�������1
            ��recv����-1����errno == EAGAIN���ú�������0 ����ú�������-1
            ��������������⣬�ú�������0

            */
            if(FD_ISSET(i, &readset)) {
                r = onSocketRead(i, buffer_array[i]);
            }

            if(r == 0 && FD_ISSET(i, &writeset)) {
                //r = onSocketWrite(i, buffer_array[i]);
                while(buffer_array[i]->readIndex < buffer_array[i]->writeIndex) {
                    // ������Ҫ���з����ͱ��������������޷����� size_t��������
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

            // �� r==1ʱ��˵���Զ˹ر������ӣ���ʱӦ�õ���close��������buffer����
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