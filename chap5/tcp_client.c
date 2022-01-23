#include"Common.h"

#define MESSAGE_SIZE 10240000

// �ͻ�����Ҫ��������˷�������
// ����ͻ��˷��͵���һ����Ϊ MESSAGE_SIZE ���ַ�����
void send_data (int fd) 
{
    // ��ʼ������һ����Ϊ MESSAGE_SIZE ���ַ�����
    char* mess;
    mess = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; ++i) 
    {
        mess[i] = 'z';
    }
    mess[MESSAGE_SIZE] = '\0';

    const char* p;
    p = mess;
    size_t remain = strlen(mess); // ��¼ʣ�����ݵĳ��ȣ�����==0ʱ����������
    while (remain)
    {
        int result = send(fd, p, remain, 0);  // ����ʱ�������ǽ����ݷ��͵����ͻ������У�������ζ�����ӶԶ��Ѿ��յ����е�����
        fprintf(stdout, "send to buffer %ld \n", result);
        if (result <= 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }

        remain -= result;
        p += result;
    }
    
    return;
}


// ip��ַͨ����������ʽ���� main ����
int main (int argc, char **argv) 
{
    if (argc != 2) // û�д���ip��ַ�������������
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }
    
    int client_sock;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(9819); // �󶨷������˿�
    inet_pton(AF_INET, argv[1], &server.sin_addr);  // ���ַ������͵�ip��ַ��ת�����������紫�����ֵ��ʽ

    int c = connect(client_sock, (struct sockaddr*) &server, sizeof(server));
    if (c < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    
    send_data(client_sock);
    exit(0);

}