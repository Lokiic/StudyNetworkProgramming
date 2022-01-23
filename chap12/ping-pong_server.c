/*

实现功能：
1. 服务器端可以接受一个参数（sleepingtime），用来模拟连接没有响应的情况
2. 服务器端程序在接收到客户端发送来的各种消息后，进行处理，
   其中如果发现是 PING 类型的消息，在休眠一段时间后回复一个 PONG 消息

*/

#include "Common.h"
#include "message_object.h"

static int count;

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("parameter wrong");
        exit(EXIT_FAILURE);
    }

    int sleepingtime = atoi(argv[1]);

    int listenfd, connectfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 1024) < 0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    connectfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }

    message_object mess;

    count = 0; // 记录报文个数

    for (;;)
    {
        int n = read(connectfd, (char *) &mess, sizeof(mess));
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            perror("client quit");
            exit(EXIT_FAILURE);
        }

        printf("received %d bytes \n", n);
        ++count;

        switch (ntohl(mess.type))
        {
        case TYPE1:
            printf("received type1 message \n");
            break;

        case TYPE2:
            printf("received type2 message \n");
            break;

        case PING: // 收到ping包后需要回一个pong包
        {
            message_object mess_server;
            mess_server.type = htonl(PONG);
            sleep(sleepingtime); // 可以模拟服务器未响应

            int c = send(connectfd, (char*) &mess_server, sizeof(mess_server), 0);
            if (c < 0)
            {
                perror("send fail");
                exit(EXIT_FAILURE);
            }
            break;
        }
            
        default: 
            printf("unknown message type %d \n", ntohl(mess.type));
            exit(EXIT_FAILURE);
            break;
        }
    }
    
    
    
}