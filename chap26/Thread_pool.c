/*

ʵ�ֹ��ܣ�
�����̵߳�Ƶ�����������٣�������С�Ŀ���������ͨ��Ԥ�����̳߳صķ�ʽ�����߳�
1. ������������ʱ��Ԥ�����̳߳أ�
   ���������ӽ���ʱ���������ֶ����������������������֣��̳߳�����̸߳���������ֶ�����ȡ�����������ֽ��д���

2. ��ƺ���������ֶ��У��������г�ʼ���������������һ��������fd���Ӷ��������������fd��
3. ��ƶ���ʱע��ʹ�� ������ �� �������� 

*/

#include "Common.h"
#define QUEUE_MAXSIZE 128
#define THREAD_NUMBER 4

extern void echo(int fd);

// �߳�
typedef struct {
    pthread_t tid;
    long thread_count;  /* # connections handled */
}Thread;


// �����ֶ���
typedef struct {
    int maxSize; // ������������������
    int *connectfd; // ���������������
    int front; // ���е�ͷλ��
    int rear;  // ���е�βλ��
    pthread_mutex_t mutex; // ������
    pthread_cond_t cond;  // ��������
}ConnectQueue;

// �����ֶ��г�ʼ��
void ConnectQueue_init(ConnectQueue* cq, int size) {
    cq->maxSize = size;
    cq->connectfd = calloc(size, sizeof(int));
    cq->front = 0;
    cq->rear = 0;
    pthread_mutex_init(&cq->mutex, NULL);
    pthread_cond_init(&cq->cond, NULL);
}

// �������ֶ����з���һ���ļ�������
void ConnectQueue_push(ConnectQueue* cq, int fd) {
    // �����ȼ�������Ϊ�����ж���߳���ͬʱ��д����
    pthread_mutex_lock(&cq->mutex);
    // ���������ŵ���βλ�ã����rear�����β����������Ϊ0
    cq->connectfd[cq->rear] = fd;
    if(++cq->rear == cq->maxSize) {
        cq->rear = 0;
    }
    printf("push fd %d \n", fd);
    // ��������������Ҫ֪ͨ�����߳����µ��������������������
    pthread_cond_signal(&cq->cond);
    pthread_mutex_unlock(&cq->mutex);
}

// �������ֶ�����ȡ��һ���ļ�������
int ConnectQueue_pop(ConnectQueue* cq) {
    // �����ȼ�������Ϊ�����ж���߳���ͬʱ��д����
    pthread_mutex_lock(&cq->mutex);

    // ���в��������жϣ�����������������wait����(whileѭ��)
    // ��front == rear ʱ��˵��û���µ��ļ�����������ʱ��Ҫ�ȴ�
    while(cq->front == cq->rear) {
        pthread_cond_wait(&cq->cond, &cq->mutex);
    }

    // ����������ʱ��ȡ���ļ�������
    int fd = cq->connectfd[cq->front];
    // ��front���ƶ�1λ������ﵽ�˶���β���ͽ�front����Ϊ0
    if(++cq->front == cq->maxSize) {
        cq->front = 0;
    }
    printf("pop fd %d \n", fd);
    pthread_mutex_unlock(&cq->mutex);
    return fd; // ���ػ�ȡ���ļ�������
}

void* thread_run(void* arg) {
    pthread_detach(pthread_self());
    ConnectQueue* cq = (ConnectQueue*) arg;
    while(1) {
        int fd = ConnectQueue_pop(cq);
        printf("%ld thread get %d fd \n", pthread_self(), fd);
        echo(fd);
    }
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

    ConnectQueue CQ;
    ConnectQueue_init(&CQ, QUEUE_MAXSIZE);

    // Ԥ�����̳߳�
    Thread* ThreadPool = calloc(THREAD_NUMBER, sizeof(Thread));
    for(int i = 0; i < THREAD_NUMBER; ++i) {
        pthread_create(&(ThreadPool[i].tid), NULL, &thread_run, (void*) &CQ);
    }

    while(1) {
        struct sockaddr_storage client;
        bzero(&client, sizeof(client));
        socklen_t clientLen = sizeof(client);
        int fd = accept(listenfd, (struct sockaddr*) &client, &clientLen);
        if(fd < 0) {
            error(1, errno, "accept fail");
        }
        else {
            ConnectQueue_push(&CQ, fd);
        }
    }
}