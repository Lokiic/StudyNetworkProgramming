/*

实现功能：
在客户端模拟TCP Keep-Alive 的机制
1. 在保活时间达到后，探活次数增加 1
2. 同时向服务器端发送 PING 格式的消息，
3. 此后以预设的保活时间间隔，不断地向服务器端发送 PING 格式的消息。
4. 如果能收到服务器端的应答，则结束保活，将保活时间置为 0。

*/


#include "Common.h"
#include "message_object.h"
#define KEEP_ALIVE_TIME 10  // 保活时间 
#define KEEP_ALIVE_INTERVAL 3  // 保活时间间隔
#define KEEP_ALIVE_PROBETIMES 3  // 保活探测次数


int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }
    
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &ser_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    
    message_object mess; // 消息对象
    message_object mess_recv; // 用于接收服务器端消息

    char recv_buf[BUFFER_SIZE];

    struct timeval tv; // 计时器
    tv.tv_sec = KEEP_ALIVE_TIME;
    tv.tv_usec = 0;

    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&allreads);
    FD_SET(sockfd, &allreads);

    int heartbeats = 0; // 记录发送心跳包的个数，达到保活探测次数时，断开连接
    for (;;)
    {
        readmask = allreads;
        int sc = select(sockfd + 1, &readmask, NULL, NULL, &tv);
        if (sc < 0)
        {
            perror("select fail");
            exit(EXIT_FAILURE);
        }
        
        if (sc == 0) // 说明在tv定义的时间内，没有任何文件描述符就绪，此时发送PING包
        {
            ++heartbeats;
            if (heartbeats > KEEP_ALIVE_PROBETIMES)
            {
                perror("TCP dead");
                exit(EXIT_FAILURE);
            }
            printf("sending heartbeats #%d \n", heartbeats);
            mess.type = htonl(PING);
            // 注意这里发送结构体的话，需要将结构体强制转换为(char *)
            int c = send(sockfd, (char *) &mess, sizeof(mess), 0); 
            if (c < 0)
            {
                perror("send fail");
                exit(EXIT_FAILURE);
            }

            tv.tv_sec = KEEP_ALIVE_INTERVAL; // 需要将计时器设置为保活时间间隔
            continue;
        }

        // 如果sc > 0 说明收到了服务器端的消息
        // if (FD_ISSET(sockfd, &readmask))
        // {
        //     int n = read(sockfd, recv_buf, BUFFER_SIZE);
        //     if (n < 0)
        //     {
        //         perror("read fail");
        //         exit(EXIT_FAILURE);
        //     }
        //     else if (n == 0) // 说明读到了EOF
        //     {
        //         peror("server quit");
        //         exit(EXIT_FAILURE);
        //     }

        //     printf("received heartbeats, make heartbeats to 0 \n");
               // 即然收到了消息，说明此连接是活跃的，那么就重置保活时间，和已经发送的探测包的个数
               // 其实应该对消息进行解析，只有是PONG类型的消息，才进行重置
               // 这里为了简便，默认收到的消息是PONG类型的
        //     tv.tv_sec = KEEP_ALIVE_TIME;
        //     heartbeats = 0;
        // }
        
        // 判断消息类型是否是PONG类型的
        if (FD_ISSET(sockfd, &readmask))
        {
            int n = read(sockfd, (char *) &mess_recv, sizeof(mess_recv));
            if (n < 0)
            {
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if (n == 0)
            {
                perror("server quit");
                exit(EXIT_FAILURE);
            }

            // 可以用switch判断多种消息类型，这里只对PONG类型判断
            if (ntohl(mess_recv.type) == PONG)
                printf("receive PONG, make heartbeats to 0 \n");
            

            tv.tv_sec = KEEP_ALIVE_TIME;
            heartbeats = 0;
            
            
        }
        
        
    }
}