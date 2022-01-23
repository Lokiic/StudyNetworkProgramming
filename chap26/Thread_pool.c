/*

实现功能：
由于线程的频繁创建和销毁，会引起不小的开销，所以通过预创建线程池的方式分配线程
1. 服务器端启动时，预创建线程池，
   当有新连接建立时，往连接字队列里放置这个新连接描述字，线程池里的线程负责从连接字队列里取出连接描述字进行处理

2. 设计合理的连接字队列（包括队列初始化、往队列里放入一个描述符fd、从队列里读出描述符fd）
3. 设计队列时注意使用 互斥锁 和 条件变量 

*/

#include "Common.h"
#define QUEUE_MAXSIZE 128
#define THREAD_NUMBER 4

extern void echo(int fd);

// 线程
typedef struct {
    pthread_t tid;
    long thread_count;  /* # connections handled */
}Thread;


// 连接字队列
typedef struct {
    int maxSize; // 队列中描述符最大个数
    int *connectfd; // 存放描述符的数组
    int front; // 队列的头位置
    int rear;  // 队列的尾位置
    pthread_mutex_t mutex; // 互斥锁
    pthread_cond_t cond;  // 条件变量
}ConnectQueue;

// 连接字队列初始化
void ConnectQueue_init(ConnectQueue* cq, int size) {
    cq->maxSize = size;
    cq->connectfd = calloc(size, sizeof(int));
    cq->front = 0;
    cq->rear = 0;
    pthread_mutex_init(&cq->mutex, NULL);
    pthread_cond_init(&cq->cond, NULL);
}

// 往连接字队列中放入一个文件描述符
void ConnectQueue_push(ConnectQueue* cq, int fd) {
    // 必须先加锁，因为可能有多个线程在同时读写队列
    pthread_mutex_lock(&cq->mutex);
    // 将描述符放到队尾位置，如果rear到达队尾，则将其重置为0
    cq->connectfd[cq->rear] = fd;
    if(++cq->rear == cq->maxSize) {
        cq->rear = 0;
    }
    printf("push fd %d \n", fd);
    // 放入描述符后，需要通知其他线程有新的连接字描述符加入队列
    pthread_cond_signal(&cq->cond);
    pthread_mutex_unlock(&cq->mutex);
}

// 从连接字队列中取出一个文件描述符
int ConnectQueue_pop(ConnectQueue* cq) {
    // 必须先加锁，因为可能有多个线程在同时读写队列
    pthread_mutex_lock(&cq->mutex);

    // 进行布尔条件判断，并调用条件变量的wait函数(while循环)
    // 当front == rear 时，说明没有新的文件描述符，此时需要等待
    while(cq->front == cq->rear) {
        pthread_cond_wait(&cq->cond, &cq->mutex);
    }

    // 当条件满足时，取出文件描述符
    int fd = cq->connectfd[cq->front];
    // 将front后移动1位，如果达到了队列尾，就将front重置为0
    if(++cq->front == cq->maxSize) {
        cq->front = 0;
    }
    printf("pop fd %d \n", fd);
    pthread_mutex_unlock(&cq->mutex);
    return fd; // 返回获取的文件描述符
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

    // 预创建线程池
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