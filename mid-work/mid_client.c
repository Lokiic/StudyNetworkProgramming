/*

ʵ�ֹ��ܣ�
1. ����ָ�������ӵķ������� IP ��ַ�Ͷ˿ڡ�
2. ������һ������֮�󣬻س�������֮��ȴ��������˽�ִ�н�����أ��ͻ��˳�����Ҫ�������ʾ����Ļ�ϡ�
3. ����������quit����ô�ͻ����˳�

*/

#include "Common.h"

// ��Ҫָ��ip��ַ�Ͷ˿ڣ������������������Ӧ��Ϊ3
int main (int argc, char** argv) {
    if (argc != 3) {
        perror("error input");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    int port = atoi(argv[2]);
    server.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    

    char recv_buf[BUFFER_SIZE];
    char send_line[MAXLINE];

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(sockfd, &allreads);

    for (;;) {
        //  ��Ϊÿ��select������ͻ�Ѵ����fd_set�޸ĵ���
        //  �Ա��֪Ӧ�ó���ʲô�����¼������ˣ�
        //  ������Ҫһ����ʼ����fd_set�������Ȥ�Ĵ����fd_set��
        readmask = allreads; 
        // �����һ������timeout����NULL��select��һֱ������ֱ��ĳ���ļ�����������
        int n = select(sockfd + 1, &readmask, NULL, NULL, NULL); 
        if (n < 0) {
            perror("select fail");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(sockfd, &readmask)) {
            bzero(recv_buf, BUFFER_SIZE);
            int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            if (rt < 0){
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if (rt == 0) {
                printf("server closed");
                exit(0);
            }
            recv_buf[rt] = 0;
            fputs(recv_buf, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(0, &readmask)) {
            bzero(send_line, MAXLINE);
            if (fgets(send_line, MAXLINE, stdin) != NULL){
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n')
                    send_line[i - 1] = 0;
                
                if (strncmp(send_line, "quit", strlen("quit")) == 0) {
                    if (shutdown(sockfd, SHUT_WR) == -1) {
                        perror("shutdown fail");
                        exit(EXIT_FAILURE);
                    }
                }

                int wt = write(sockfd, send_line, strlen(send_line));
                if (wt < 0)
                {
                    perror("write fail");
                    exit(EXIT_FAILURE);
                }
            }
            
        }
        
    }
    
}