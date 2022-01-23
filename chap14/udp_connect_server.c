/*

ʵ�ֹ��ܣ�
1. ͨ���źŴ���������Ӧ"Ctrl + C" ����
2. ͨ��recvfrom�õ��ͻ�����Ϣ��֮������connect ��UDP socket�Ϳͻ��� IPv4 ��ַ��
3. �Կͻ��˷��͵���Ϣ����"GO!"ǰ׺�������͸��ͻ���
4. ���ϵشӿͻ��˽��ձ��ģ��ù���һֱ������ֱ���ͻ��˷���"goodbye"����Ϊֹ��

*/

#include "Common.h"

static int count;

void sighandler(int para)
{
    printf("received %d datagrams \n", count);
    exit(0);
}

int main (int argc, char** argv)
{
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
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    count = 0;
    signal(SIGINT, sighandler);


    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    char recv_buf[BUFFER_SIZE];
    //bzero(recv_buf, BUFFER_SIZE);
    char send_line[MAXLINE];

    // Ҫ����recvfrom�����õ��ͻ��˵�ַ��Ϣ��Ȼ��ͨ��connect��
    int rt = recvfrom(sockfd, recv_buf, BUFFER_SIZE, 0, (struct sockaddr*) &client, &client_len);
    if (rt < 0)
    {
        perror("recv error");
        exit(EXIT_FAILURE);
    }
    printf("received %d bytes: %s \n", rt, recv_buf);
    ++count;

    if (connect(sockfd, (struct sockaddr*) &client, client_len) < 0)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    


    while (strncmp(recv_buf, "goodbye", 7) != 0) 
    {
        bzero(send_line, MAXLINE);
        sprintf(send_line, "GO! %s", recv_buf);

        size_t st = send(sockfd, send_line, strlen(send_line), 0);
        if (st < 0)
        {
            perror("send error");
            exit(EXIT_FAILURE);
        }
        printf("send %zu bytes \n", st);

        bzero(recv_buf, BUFFER_SIZE);
        size_t rc = recv(sockfd, recv_buf, BUFFER_SIZE, 0);
        if (rc < 0)
        {
            perror("recv error");
            exit(EXIT_FAILURE);
        }
        
        ++count;
        
    }
    
    
    
}