/*
 * @Author: your name
 * @Date: 2020-11-29 13:43:32
 * @LastEditTime: 2020-11-29 15:21:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\echoservert.cpp
 */
#include "csapp.h"

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char** argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        // 这个地方一定要分配在堆上。不然这个地方的connfdp赋值的时候会有线程竞争的问题，
        // 使用了堆上分配，每一次的clientlen都是新的
        connfdp = (int *)Malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Pthread_create(&tid, NULL, thread, connfdp);
    }
}

void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    // 必须分离每个线程，使得终止的时候内存资源能够被回收
    Pthread_detach(pthread_self());
    Free(vargp);
    echo(connfd);
    Close(connfd);
    return NULL;
}