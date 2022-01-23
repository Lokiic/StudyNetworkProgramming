/*

ʵ�ֹ��ܣ�
�ͻ��˿���ͨ��һЩ����ͷ���������
1. �ͻ������� pwd�� ��ʾ������Ӧ�ó�������ʱ�ĵ�ǰ·��
2. cd���ı������Ӧ�ó���ĵ�ǰ·����
3. ls����ʾ������Ӧ�ó���ǰ·���µ��ļ��б�
4. quit���ͻ��˽����˳������Ƿ������˲����˳����ڶ����ͻ������ٴ������Ϸ������ˡ�(�ͻ��˽��յ�quit�����˳�����������read==0)
����ʱ����Ҫ���Ƕ���ͻ��������ӵ����Σ�ֻ����ÿ�η���һ���ͻ����ӡ���

*/

#include "Common.h"

char* run_command (char* cmd) {
    char* data = malloc(20000);
    bzero(data, sizeof(data)); // ÿ�ε��ö�Ӧ�����㣬��ֹ֮ǰ�����ݲд���data��
    char* data_index = data;
    FILE* f;
    // ÿ�ζ�ȡ256�ֽڣ�ֱ��������������
    const int read_buffer = 256;
    char temp_buf[read_buffer];
    f = popen(cmd, "r");
    if (f) { // ȷ��popen�ɹ�ִ��
        while (!feof(f)) { // �ж��Ƿ�������ļ�ĩβ
            if (fgets(temp_buf, read_buffer, f) != NULL)
            {
                int len = strlen(temp_buf);
                memcpy(data_index, temp_buf, len);
                data_index += len;
            }
        }
        pclose(f); // �����ļ�ĩβʱ���ر��ļ�
    }
    return data;
}

int main (int argc, char** argv) {
    int listenfd, connectfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
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

    if (bind(listenfd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 1024) < 0) {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }

    
    
    // ���ڿͻ����˳�ʱ���������˲����˳���������Ҫ������һ���ͻ���
    // ����accept����Ӧ�÷��� ѭ�� ��
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);   

    char recv_buf[BUFFER_SIZE];
    char send_line[MAXLINE];

    while (1) {
        connectfd = accept(listenfd, (struct sockaddr*) &client, &client_len);
        if (connectfd < 0) {
            perror("accept fail");
            exit(EXIT_FAILURE);
        }
        while (1) {
            bzero(recv_buf, BUFFER_SIZE);
            bzero(send_line, MAXLINE);
            int rt = read(connectfd, recv_buf, BUFFER_SIZE);
            if (rt < 0)
            {
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if (rt == 0) {
                printf("client closed");
                close(connectfd);
                break;
            }
            recv_buf[rt] = 0;
            if (strncmp(recv_buf, "pwd", strlen("pwd")) == 0)
            {
                // getcwd������ȡ�õ�ǰ�Ĺ���Ŀ¼
                getcwd(recv_buf, BUFFER_SIZE);
                if (send(connectfd, recv_buf, strlen(recv_buf), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
            }
            else if (strncmp(recv_buf, "cd", strlen("cd")) == 0) {
                // ������Ҫ�����ƶ�3λ������Ϊ����cd��ʱ���� "cd ../server" cd�����пո�
                memcpy(send_line, recv_buf + 3, strlen(recv_buf) - 3);
                if (chdir(send_line) == -1) {
                    printf("change dir failed, %s \n", send_line);
                }
                printf("now dir is: %s \n", send_line);
                if (send(connectfd, send_line, strlen(send_line), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
            }
            else if (strncmp(recv_buf, "ls", strlen("ls")) == 0)
            {
                char* result = run_command("ls");
                if (send(connectfd, result, strlen(result), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
                free(result);
            }
            else {
                // �ͻ��������ָ��ԣ���Ҫ����
                char *s = "error: unknown instruction!";
                if (send(connectfd, s, strlen(s), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
            }
                   
        }
    }

}