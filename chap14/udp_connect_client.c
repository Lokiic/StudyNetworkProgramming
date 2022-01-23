/*

ʵ�ֹ��ܣ�
1. ͨ��connect��UDP socket �� IPv4 ��ַ���������ˣ����а�
2. ��ȡ��׼�����ַ��������͸��Զˣ�ͨ��send������
3. �ѶԶ���Ӧ��Ϣ��ӡ����׼�����ͨ��recv������

*/

#include "Common.h"

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("make socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    
    
    char send_line[MAXLINE];
    char recv_buf[BUFFER_SIZE];

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        bzero(recv_buf, BUFFER_SIZE);
        int n = strlen(send_line);
        if (send_line[n - 1] == '\n')
            send_line[n - 1] = 0;
        
        printf("now sending %s \n", send_line);
        size_t st = send(sockfd, send_line, strlen(send_line), 0);
        if (st < 0)
        {
            perror("send error");
            exit(EXIT_FAILURE);
        }
        printf("sending bytes: %zu \n", st);

        int rt = recv(sockfd, recv_buf, BUFFER_SIZE,  0);
        if (rt < 0)
        {
            perror("recv error");
            exit(EXIT_FAILURE);
        }

        fputs(recv_buf, stdout);
        fputs("\n", stdout);
        
    }
    
    exit(0);
    
}