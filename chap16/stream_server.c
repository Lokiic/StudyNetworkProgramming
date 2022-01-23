/*

ʵ�ֹ��ܣ�
1. ѭ�������յ����ֽ��������б��Ľ�������
2. ʵ�� read_message ���������б��Ľ���
3. �����������ӡ����׼�����

*/

#include "Common.h"

static int count;
void handler (int para)
{
    printf("received %d datagrams \n", count);
    exit(0);
}

// ��ȡԤ���С(length)���ֽ�
size_t readn (int fd, void* buffer, size_t length)
{
    char* ptr = buffer;
    size_t count = length;

    while (count > 0)
    {
        size_t n = read(fd, ptr, count);
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
            break; // EOF
        
        count -= n;
        ptr += n;
    }
    
    return length - count; // ����ʵ�ʶ�ȡ���ֽ���
}

size_t read_message (int fd, char* buffer, size_t length)
{
    u_int32_t message_length;
    u_int32_t message_type;

    size_t rt;
    rt = readn(fd, (char*) &message_length, sizeof(message_length));
    if (rt != sizeof(message_length))
        return rt < 0 ? -1 : 0;  // rt < 0˵������ rt == 0 ˵�� EOF
    message_length = ntohs(message_length);

    rt = readn(fd, (char*) &message_type, sizeof(message_type));
    if (rt != sizeof(message_type))
        return rt < 0 ? -1 : 0;  // rt < 0˵������ rt == 0 ˵�� EOF
    message_type = ntohs(message_type);

    if (message_length > length)  // �����Ϣ���ȴ��ڻ��������ȣ�ֱ�ӱ���
        return -1;
    
    rt = readn(fd, buffer, message_length);
    if (rt != message_length)
        return rt < 0 ? -1 : 0;
    
    return rt;
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
    
    count = 0;
    signal(SIGINT, handler);
    char buf[128];

    for (;;)
    {
        bzero(buf, sizeof(buf));
        size_t rt = read_message(connectfd, buf, sizeof(buf));
        if (rt < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (rt == 0)
        {
            printf("client quit");
            exit(0);
        }

        printf("received %zu bytes: %s", rt, buf);
        ++count;
    }

}