/*

实现功能：
客户端可以通过一些命令和服务器交互
1. 客户端输入 pwd， 显示服务器应用程序启动时的当前路径
2. cd：改变服务器应用程序的当前路径。
3. ls：显示服务器应用程序当前路径下的文件列表
4. quit：客户端进程退出，但是服务器端不能退出，第二个客户可以再次连接上服务器端。(客户端接收到quit，会退出，服务器端read==0)
（暂时不需要考虑多个客户并发连接的情形，只考虑每次服务一个客户连接。）

*/

#include "Common.h"

char* run_command (char* cmd) {
    char* data = malloc(20000);
    bzero(data, sizeof(data)); // 每次调用都应该清零，防止之前的数据残存在data中
    char* data_index = data;
    FILE* f;
    // 每次读取256字节，直到读完所有数据
    const int read_buffer = 256;
    char temp_buf[read_buffer];
    f = popen(cmd, "r");
    if (f) { // 确保popen成功执行
        while (!feof(f)) { // 判断是否读到了文件末尾
            if (fgets(temp_buf, read_buffer, f) != NULL)
            {
                int len = strlen(temp_buf);
                memcpy(data_index, temp_buf, len);
                data_index += len;
            }
        }
        pclose(f); // 读到文件末尾时，关闭文件
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

    
    
    // 由于客户端退出时，服务器端不能退出，而是需要连接下一个客户端
    // 所以accept函数应该放在 循环 里
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
                // getcwd函数，取得当前的工作目录
                getcwd(recv_buf, BUFFER_SIZE);
                if (send(connectfd, recv_buf, strlen(recv_buf), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
            }
            else if (strncmp(recv_buf, "cd", strlen("cd")) == 0) {
                // 这里需要往后移动3位，是因为调用cd的时候是 "cd ../server" cd后面有空格
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
                // 客户端输入的指令不对，需要报错
                char *s = "error: unknown instruction!";
                if (send(connectfd, s, strlen(s), 0) < 0) {
                    perror("send fail");
                    exit(EXIT_FAILURE);
                }
            }
                   
        }
    }

}