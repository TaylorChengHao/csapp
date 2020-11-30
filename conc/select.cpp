/*
 * @Author: your name
 * @Date: 2020-11-29 11:36:56
 * @LastEditTime: 2020-11-29 13:08:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\select.cpp
 */
#include "csapp.h"
void echo(int connfd);
void command(void);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
    
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);        //把stdin加到read_set中
    FD_SET(listenfd, &read_set);            //把监听描述符加到read_set中

    while (1)
    {
        ready_set = read_set;
        Select(listenfd+1, &ready_set, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &ready_set))
            command();                      //从控制台中读行
        if (FD_ISSET(listenfd, &ready_set))
        {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            echo(connfd);                   //Echo client input until EOF
            Close(connfd);
        }
    }
}

void command(void){
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
        exit(0);
    printf("%s",buf);                       //处理输入的字符
}