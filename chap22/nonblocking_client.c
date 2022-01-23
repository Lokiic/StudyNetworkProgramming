/*

ʵ�ֹ��ܣ�
���Է������˵�accept�Ƿ������
ͨ������SO_LINGERѡ������ӽ�������������close���۲�������˵ķ�Ӧ

*/

#include "Common.h"

int main (int argc, char** argv) {
    if(argc != 2) {
        error(1, 0, "ip error");
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        error(1, errno, "make socket fail");
    }

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if(connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        error(1, errno, "connect fail");
    }

    struct linger lin;
    lin.l_onoff = 1;
    lin.l_linger = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &lin, sizeof(lin));
    close(sockfd);

    exit(0);
}