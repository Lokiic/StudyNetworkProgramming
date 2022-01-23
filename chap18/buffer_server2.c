/*

ʵ�ֹ��ܣ�
���ķֽ����'\n'
1. ������Ǳ��ķֽ���������ݿ�����������
2. ����Ǳ��ķֽ�����򽫻������ضϣ��������ն�ȡ���ַ�����

*/

#include "Common.h"

size_t readline(int fd, char* buf, size_t length)
{
    char* buf_first = buf; // ��¼������һ��ʼ��λ�ã������������ַ��ĸ���
    char* buf_temp;
    char temp[512];
    char c;
    int sum = 0;
    while (--length > 0)
    {
        if (sum <= 0)
        {
            int nread = recv(fd, temp, sizeof(temp), 0);
            if (nread < 0)
            {
                if (errno == EINTR)
                {
                    ++length;
                    continue;
                }
                else
                    return -1;
            }
            else if (nread == 0)
                return 0;
            
            sum = nread;
            buf_temp = temp;
        }
        // ��ȡ��ʱ���������ַ������俽����Ҫ�洢�Ļ�������
        c = *buf_temp;
        ++buf_temp;

        *buf = c;
        ++buf;
        --sum;
        if (c == '\n') // ����Ƿֽ������Ҫ�����������ϣ����������ն�ȡ���ַ�����
        {
            *buf = '\0';
            return buf - buf_first;
        }
        
    }
    return -1; // ��Ϣ������Ӧ������Ϣ���Ӧ����'\n'��β�����ڻ������м���'\0'
}




int main (int argc, char** argv)
{
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

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

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

    char recv_buf[10];

    while (1)
    {
        bzero(recv_buf, 10);
        int n = readline(connectfd, recv_buf, 10);
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            printf("client closed");
            exit(0);
        }
        printf("received %d bytes: %s \n", n, recv_buf);
        
    }
    
}    