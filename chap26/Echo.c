#include "Common.h"

char ROT13(char c) {
    if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    return c;
}

void echo(int fd) {
    char recv_buf[BUFFER_SIZE];
    bzero(recv_buf, BUFFER_SIZE);

    size_t readIndex = 0;
    while(1) {
        char ch;
        int rt = recv(fd, &ch, 1, 0);
        if(rt == 0) {
            break;
        }
        else if(rt < 0) {
            error(1, errno, "recv fail");
        }

        // 读到 '\n' 表明客户端的一段数据已经读完，此时可以将编码后的数据发送给客户端
        if(ch == '\n') {
            int st = send(fd, recv_buf, readIndex, 0);
            if(st < 0) {
                error(1, errno, "send fail");
            }
            readIndex = 0; // 复用recv_buf
            continue;
        }
        // 如果不是'\n'，那么将ch编码并存入recv_buf
        if(readIndex < BUFFER_SIZE)
            recv_buf[readIndex++] = ROT13(ch);  
    }
}