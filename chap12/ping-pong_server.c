/*

ʵ�ֹ��ܣ�
1. �������˿��Խ���һ��������sleepingtime��������ģ������û����Ӧ�����
2. �������˳����ڽ��յ��ͻ��˷������ĸ�����Ϣ�󣬽��д���
   ������������� PING ���͵���Ϣ��������һ��ʱ���ظ�һ�� PONG ��Ϣ

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

    count = 0; // ��¼���ĸ���

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

        case PING: // �յ�ping������Ҫ��һ��pong��
        {
            message_object mess_server;
            mess_server.type = htonl(PONG);
            sleep(sleepingtime); // ����ģ�������δ��Ӧ

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