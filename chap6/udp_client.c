#include"Common.h"
#define MAXLINE  4096

/*

ʵ�ֹ��ܣ�
1. �ӱ�׼�����ж�ȡ������ַ����������͸���������
2. �ѷ���˾�������ı��Ĵ�ӡ����׼�����

*/

// socket->sendto->recvfrom

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
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_adr;
    bzero(&serv_adr, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &serv_adr.sin_addr);

    char send_line[MAXLINE];
    char recv_line[MAXLINE + 1];

    // Ϊ�˱���recvfrom�����õ��ĶԶ˵�ַ��Ϣ������ԭserv_addr(ʵ����û�иı�)��
    // ����Ӧ�ù���һ��ָ��ͨ������sockaddr��ָ�룬��ŶԶ˵ĵ�ַ��Ϣ
    struct sockaddr* temp_addr;
    temp_addr = malloc(sizeof(serv_adr));
    socklen_t temp_len;

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int n = strlen(send_line); // ����fgetsֻ�ܶ�ȡһ�����ݣ�������Ҫ�ж϶������ݵ����һ���ַ��Ƿ��ǻ��з�
        if (send_line[n-1] == '\n')
        {
            send_line[n-1] = 0;
        }
        
        printf("now sending %s \n", send_line);
        size_t send_t = sendto(sockfd, send_line, n, 0, (struct sockaddr*) &serv_adr, sizeof(serv_adr));
        if (send_t < 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        } 
        printf("send %zu bytes \n", send_t);

        temp_len = sizeof(serv_adr);
        size_t recv_t = recvfrom(sockfd, recv_line, MAXLINE, 0, (struct sockaddr*) &temp_addr, &temp_len);
        if (recv_t < 0)
        {
            perror("recvfrom fail");
            exit(EXIT_FAILURE);
        }

        recv_line[recv_t] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
        
    }    

    exit(0);
}