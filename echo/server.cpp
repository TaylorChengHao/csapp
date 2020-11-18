/*
 * @Author: your name
 * @Date: 2020-11-17 11:20:52
 * @LastEditTime: 2020-11-17 15:08:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\echo\server.cpp
 */
#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;         //存放地址的空间
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr,clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connect to (%s, %s)\n", client_hostname, client_port);
        
        // 业务逻辑
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}