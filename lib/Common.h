#ifndef MANU_COMMON_H
#define MANU_COMMON_H

#include    <sys/types.h>    /* basic system data types */
#include    <sys/socket.h>    /* basic socket definitions */
#include    <sys/time.h>    /* timeval{} for select() */
#include    <time.h>        /* timespec{} for pselect() */
#include    <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include    <arpa/inet.h>    /* inet(3) functions */
#include    <errno.h>
#include    <fcntl.h>        /* for nonblocking */
#include    <netdb.h>
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/stat.h>    /* for S_xxx file mode constants */
#include    <sys/uio.h>        /* for iovec{} and readv/writev */
#include    <unistd.h>
#include    <sys/wait.h>
#include    <sys/un.h>        /* for Unix domain sockets */
#include    <sys/select.h>
#include    <poll.h>
#include    <sys/epoll.h>
#include    <sys/wait.h>
#include    <pthread.h>


void error(int status, int err, char* format, ...);


#define     SERVER_PORT     9819
#define     LISTEN_BACKLOG  1024
#define     MAXLINE         4096
#define     BUFFER_SIZE     4096

#endif