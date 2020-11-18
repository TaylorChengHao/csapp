/*
 * @Author: your name
 * @Date: 2020-11-16 21:32:07
 * @LastEditTime: 2020-11-17 11:23:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\echo\client.cpp
 */

#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];

    rio_t rio;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n",argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];
    
    // 包装了getaddrinfo（dns解析），将host转为IP地址，创建字符串socket，并connect
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while(Fgets(buf, MAXLINE, stdin) != NULL)
    {
        // 业务逻辑
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    Close(clientfd);
    exit(0);
}