/*

ʵ�ֹ��ܣ�
�ڿͻ���ģ��TCP Keep-Alive �Ļ���
1. �ڱ���ʱ��ﵽ��̽��������� 1
2. ͬʱ��������˷��� PING ��ʽ����Ϣ��
3. �˺���Ԥ��ı���ʱ���������ϵ���������˷��� PING ��ʽ����Ϣ��
4. ������յ��������˵�Ӧ����������������ʱ����Ϊ 0��

*/


#include "Common.h"
#include "message_object.h"
#define KEEP_ALIVE_TIME 10  // ����ʱ�� 
#define KEEP_ALIVE_INTERVAL 3  // ����ʱ����
#define KEEP_ALIVE_PROBETIMES 3  // ����̽�����


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
    
    message_object mess; // ��Ϣ����
    message_object mess_recv; // ���ڽ��շ���������Ϣ

    char recv_buf[BUFFER_SIZE];

    struct timeval tv; // ��ʱ��
    tv.tv_sec = KEEP_ALIVE_TIME;
    tv.tv_usec = 0;

    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&allreads);
    FD_SET(sockfd, &allreads);

    int heartbeats = 0; // ��¼�����������ĸ������ﵽ����̽�����ʱ���Ͽ�����
    for (;;)
    {
        readmask = allreads;
        int sc = select(sockfd + 1, &readmask, NULL, NULL, &tv);
        if (sc < 0)
        {
            perror("select fail");
            exit(EXIT_FAILURE);
        }
        
        if (sc == 0) // ˵����tv�����ʱ���ڣ�û���κ��ļ���������������ʱ����PING��
        {
            ++heartbeats;
            if (heartbeats > KEEP_ALIVE_PROBETIMES)
            {
                perror("TCP dead");
                exit(EXIT_FAILURE);
            }
            printf("sending heartbeats #%d \n", heartbeats);
            mess.type = htonl(PING);
            // ע�����﷢�ͽṹ��Ļ�����Ҫ���ṹ��ǿ��ת��Ϊ(char *)
            int c = send(sockfd, (char *) &mess, sizeof(mess), 0); 
            if (c < 0)
            {
                perror("send fail");
                exit(EXIT_FAILURE);
            }

            tv.tv_sec = KEEP_ALIVE_INTERVAL; // ��Ҫ����ʱ������Ϊ����ʱ����
            continue;
        }

        // ���sc > 0 ˵���յ��˷������˵���Ϣ
        // if (FD_ISSET(sockfd, &readmask))
        // {
        //     int n = read(sockfd, recv_buf, BUFFER_SIZE);
        //     if (n < 0)
        //     {
        //         perror("read fail");
        //         exit(EXIT_FAILURE);
        //     }
        //     else if (n == 0) // ˵��������EOF
        //     {
        //         peror("server quit");
        //         exit(EXIT_FAILURE);
        //     }

        //     printf("received heartbeats, make heartbeats to 0 \n");
               // ��Ȼ�յ�����Ϣ��˵���������ǻ�Ծ�ģ���ô�����ñ���ʱ�䣬���Ѿ����͵�̽����ĸ���
               // ��ʵӦ�ö���Ϣ���н�����ֻ����PONG���͵���Ϣ���Ž�������
               // ����Ϊ�˼�㣬Ĭ���յ�����Ϣ��PONG���͵�
        //     tv.tv_sec = KEEP_ALIVE_TIME;
        //     heartbeats = 0;
        // }
        
        // �ж���Ϣ�����Ƿ���PONG���͵�
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

            // ������switch�ж϶�����Ϣ���ͣ�����ֻ��PONG�����ж�
            if (ntohl(mess_recv.type) == PONG)
                printf("receive PONG, make heartbeats to 0 \n");
            

            tv.tv_sec = KEEP_ALIVE_TIME;
            heartbeats = 0;
            
            
        }
        
        
    }
}