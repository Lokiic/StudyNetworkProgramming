/*
ʵ�ֹ��ܣ�
1. ���տͻ��˷��͵��ֽ���
2. �Խ��յ����ֽ�������ǰ׺ "GO!" �����ظ��ͻ���
*/
#include "Common.h"

// ��Ҫͨ�����������룬���ص�ַ(���Ե�ַ)��������һ�������ļ���·��
// �ֽ�����ʽ�ı���socket ���� TCP socket
int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("path error");
        exit(EXIT_FAILURE);
    }

    int listenfd, connectfd;
    listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    char* localpath = argv[1];
    // Ϊ�˱����ݵ��ԣ���Ҫ�ѱ��ص�ַ���ڵ��ļ�ɾ��
    // (����ÿ�����з���������ʱ�������Ȱ��ļ�ɾ����Ȼ����bind�������ļ������������ٴβ���������һ���Ľ��)
    unlink(localpath);

    struct sockaddr_un ser_addr, cli_addr;
    socklen_t cli_len;
    cli_len = sizeof(cli_addr);

    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, localpath);

    if (bind(listenfd, (struct sockaddr*) & ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }   

    if (listen(listenfd, LISTEN_BACKLOG) < 0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }
    
    connectfd = accept(listenfd, (struct sockaddr*) &cli_addr, &cli_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }
    

    char read_buf[BUFFER_SIZE];

    for (;;)
    {
        // ÿ�ν���һ��ѭ��ǰ����Ҫ��read_buf���
        bzero(read_buf, sizeof(read_buf));
        if (read(connectfd, read_buf, BUFFER_SIZE) == 0)
        {
            printf("client quit");
            break;
        }
        printf("Received: %s \n", read_buf);

        char send_line[MAXLINE];
        sprintf(send_line, "GO!  %s", read_buf);

        int n = sizeof(send_line);
        if (write(connectfd, send_line, n) != n)
        {
            perror("write error");
            exit(EXIT_FAILURE);
        }
        
    }

    close(connectfd);
    close(listenfd);

    exit(0);
    

}   
