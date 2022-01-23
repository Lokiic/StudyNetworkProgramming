/*

ʵ�ֹ��ܣ�
1. �ӱ�׼��������û����룬����������ַ���ͨ��socket���͸���������
2. ���������˵�Ӧ����ʾ�ڱ�׼�����
3. ����û�������"close",������close�����ر����ӣ���ʱ������һ��ʱ�䣬�ȴ��������˴�������˳�
4. ����û�������"shutdown"��������shutdown�����ر����ӵ�д���򣬴�ʱ����ֱ���˳������Ǽ����ȴ��������˵�Ӧ��ֱ��������������Լ��Ĳ���������һ����������ɹرա�

��ͨ��select���ж�·���ã�ͬʱ��ɶ�����socket�ͱ�׼��������I/O����Ĵ���

*/


#include"Common.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &ser_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    
    char send_line[MAXLINE];
    char recv_buf[BUFFER_SIZE];

    // fd_set �ṹ��
    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&allreads);  // ���fd_set������λ
    FD_SET(0, &allreads); // ����fd_set��λfd
    FD_SET(sockfd, &allreads);

    for (;;)
    {
        readmask = allreads;
        int r = select(sockfd + 1, &readmask, NULL, NULL, NULL); // ���ؾ������ɶ�����д���쳣���ļ�������������
        if (r <= 0)
        {
            perror("select fail");
            exit(EXIT_FAILURE);
        }
        
        // ������socket���д���
        // ��ȡ�������˷��͵����ݣ�����ʾ�ڱ�׼�����
        if (FD_ISSET(sockfd, &readmask))  // ����fd_set��λfd�Ƿ�����
        {
            int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            if (rt < 0)
            {
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if (rt == 0)  // read == 0 ��ʾEOF�����Զ˷�����FIN��
            {
                perror("server close");
                exit(EXIT_FAILURE);
            }
            recv_buf[rt] = 0;
            fputs(recv_buf, stdout);
            fputs("\n", stdout);
            
        }

        // �Ա�׼������д���
        // �ж��������ַ��������� close �� shutdown
        if (FD_ISSET(0, &readmask))
        {
            if (fgets(send_line, MAXLINE, stdin) != NULL)
            {
                // �������ַ��������ж�
                if(strncmp(send_line, "shutdown", 8) == 0) // ����������"shutdown"����رձ�׼�����I/O�¼���֪
                {
                    // �ر�д���򣬽��� "��ر�״̬"�����fd_set��λfd
                    FD_CLR(0, &allreads);
                    if (shutdown(sockfd, 1)) // shutdown���ɹ��᷵��0�����Գɹ�ʱ�������if�ڲ�
                    {
                        perror("shutdown error");
                        exit(EXIT_FAILURE);
                    }
                    
                }
                else if(strncmp(send_line, "close", 5) == 0)
                {
                    FD_CLR(0, &allreads);
                    if (close(sockfd))
                    {
                        perror("close error");
                        exit(EXIT_FAILURE);
                    }
                    // ��close�ɹ�
                    // ����һ��ʱ�䣬�ȴ��������˴�������˳�
                    sleep(6);
                    exit(0);
                }
                else // ������������ַ�������ʱӦ�ð�������ַ������͸���������
                {
                    int st = strlen(send_line);  // strlen����㻻�з�
                    if (send_line[st - 1] == '\n')  // �ص��س���
                        send_line[st - 1] = 0;
                    
                    printf("now sending: %s \n", send_line);
                    size_t wt = write(sockfd, send_line, strlen(send_line));
                    if (wt < 0)
                    {
                        perror("write fail");
                        exit(EXIT_FAILURE);
                    }
                    printf("send bytes: %zu \n", wt);
                    
                }
                
            }
            
        }
        
        
    }
    
}
