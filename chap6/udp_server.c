#include "Common.h"

/*

ʵ�ֹ��ܣ�
1. ��������ͳ���յ���UDP���ݱ��ĸ��� 
2. ����Ӧ"Ctrl + "C""ʱ���˳�������ӡ���������յ��ı�������(ͨ��signal����ʵ��)
3. ���ڿͻ��˷��������ݣ���������ÿ�����ݼ��� GO����ǰ׺

*/

/*

socket->bind->recvfrom->sendto

ssize_t recvfrom(int sockfd, void* buff, size_t nbytes, int flags, 
��������������������struct sockaddr* from, socklen_t* addrlen); 

ssize_t sendto(int sockfd, const void* buff, size_t nbytes, int flags,
                const struct sockaddr* to, socklen_t addrlen); 


*/

#define MAXLINE  4096

static int count; // ���ڼ�¼���յ���UDP���ݱ��ĸ���

void handler_function(int para)  // �źŴ�����
{   
    printf("\n received %d datagrams \n", count);
    exit(0);
}

int main (int argc, char** argv)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // UDP IPv4 socket
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ser_addr, cli_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(SERVER_PORT);
    
    bind(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));

    char message[MAXLINE];
    socklen_t cli_len = sizeof(cli_addr);
    count = 0;

    signal(SIGINT, handler_function);

    for (;;)
    {
        // ͨ���ú������Եõ��Զ˷��ͷ��ĵ�ַ�Ͷ˿ڵ���Ϣ��Ȼ��������ø���Ϣ��ͨ��sendto�������������ݸ��Զ�
        int n = recvfrom(sockfd, message, MAXLINE, 0, (struct sockaddr*) &cli_addr, &cli_len); 
        message[n] = 0;
        printf("received %d bytes: %s \n", n, message);

        char send_line[MAXLINE];
        sprintf(send_line, "GO! %s", message); // �����յ����ַ�������ǰ׺��Ȼ���ٷ��͸��ͻ���

        sendto(sockfd, send_line, strlen(send_line), 0, (struct sockaddr *) &cli_addr, cli_len);
        
        ++count; // ��ʾ���յ���UDP���ݱ�����+1


    }
    
    exit(0);
    
}