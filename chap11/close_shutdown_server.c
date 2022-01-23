/*

ʵ�ֹ��ܣ�
1. ��ӡ���ӿͻ����յ�������
2. ���յ������ݽ��и�ʽ�����ٷ��ظ��ͻ���
3. ��SIGPIPE�źŽ��д�������Զ��Ѿ��ر������ӣ���ʱ��д�����ݣ����յ�RST����������Ѿ��յ�RST��socketִ��д�������ں˻ᷢ��һ��SIGPIPE�źŸ����̣�
4. ��������ǰ������һ��ʱ�䣬ģ��������˵Ĵ���ʱ��

*/

#include"Common.h"

static int count; // ��¼�ͻ��˷��͵ı�������

static void sig_handler(int para)
{
    printf("\n received %d datagram \n", count);
    exit(0);
}

int main(int argc, char** argv)
{
    int listenfd, connectfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 1024) < 0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    connectfd = accept(listenfd, (struct sockaddr*) &cli_addr, &cli_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }
    
    char recv_buf[BUFFER_SIZE];

    count = 0;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_handler);
    for (;;)
    {
        bzero(recv_buf, BUFFER_SIZE);
        int n = read(connectfd, recv_buf, BUFFER_SIZE);
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            perror("client close");
            exit(EXIT_FAILURE);
        }

        recv_buf[n] = 0;
        ++count;
        printf("received %d bytes: %s \n", n, recv_buf);

        char send_line[MAXLINE];
        sprintf(send_line, "GO! %s", recv_buf);

        sleep(5); // ����һ��ʱ�䣬ģ��������˵Ĵ���ʱ��
        size_t wt = write(connectfd, send_line, strlen(send_line));
        if (wt < 0)
        {
            perror("write fail");
            exit(EXIT_FAILURE);
        }
        printf("send bytes: %zu \n", wt);
    }
    
}