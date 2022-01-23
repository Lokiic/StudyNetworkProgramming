/*

ʵ�ֹ��ܣ�
1. ͨ����׼�����ȡ���ݣ����������ݷ�����������
2. ���ӷ������˽��յ����ݣ���ӡ�ڱ�׼�����

*/

// ע�⣡��UDP socket ��ͬ���ǣ��������ݱ���ʽ��socket�Ŀͻ��˳�����Ҫbind�Լ��ı���·�����Ա�������ذ�ʱ��������ȷ�ҵ���ַ
// ��UDP�ͻ��˳��������������ͨ��UDP���ı��ص�ַ�Ͷ˿ڵõ��ͻ��˵ĵ�ַ��Ϣ������ UDP �ͻ��˲���Ҫ bind �˿�
#include"Common.h"

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("path error");
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_un cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));
    cli_addr.sun_family = AF_LOCAL;
    strcpy(cli_addr.sun_path, tmpnam(NULL));

    
    if (bind(sockfd, (struct sockaddr*) &cli_addr, sizeof(cli_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_un ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, argv[1]);


    char send_line[MAXLINE];
    bzero(send_line, MAXLINE);
    char recv_buf[BUFFER_SIZE];

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int n = strlen(send_line);
        if (send_line[n-1] == '\n')
        {
            send_line[n-1] = 0;
        }
        size_t nbytes = strlen(send_line);
        printf("now sending %s \n", send_line);

        if (sendto(sockfd, send_line, nbytes, 0, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) != nbytes)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }
        
        int l = recvfrom(sockfd, recv_buf, BUFFER_SIZE, 0, NULL, NULL); // ������֪�������˵ĵ�ַ��Ϣ�����Կ��Բ��������ﲶ�����ַ
        recv_buf[l] = 0;

        fputs(recv_buf, stdout);
        fputs("\n", stdout);
    }
    
    exit(0);
}