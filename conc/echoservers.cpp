/*
 * @Author: your name
 * @Date: 2020-11-18 17:26:26
 * @LastEditTime: 2020-11-18 22:51:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\echoservers.cpp
 */
#include "csapp.h"

// 用连接池管理各个客户端
typedef struct
{
    int maxfd;        //最大的文件描述符
    fd_set read_set;  //所有激活的文件描述符
    fd_set ready_set; //准备读的描述符的子集
    int nready;       //准备读的描述符的数量
    int maxi;
    int clientfd[FD_SETSIZE];    //激活的描述符集合
    rio_t clientrio[FD_SETSIZE]; //激活的读缓冲区
} pool;

void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
void check_clients(pool *p);

// 用来统计服务器接收的byte的数量
int byte_cnt = 0;

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    static pool pool;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool);

    while (1)
    {
        // 等待描述符可用
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

        // 如果监听描述符准备好了，加入新的客户端到池子中
        if (FD_ISSET(listenfd, &pool.ready_set))
        {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            add_client(connfd, &pool);
        }
        
        // 每个连接的描述符echo一行文本
        check_clients(&pool);
    }
}

// 初始化客户端池
void init_pool(int listenfd, pool *p)
{
    // 初始化
    // -1表示一个可用的槽位
    p->maxi = -1;
    for (int i = 0; i < FD_SETSIZE; i++)
        p->clientfd[i] = -1;

    // 目前只有监听字符串，没有连接字符串
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p)
{
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++)
    {
        if (p->clientfd[i] < 0)
        {
            // 增加已连接的描述符进池子
            p->clientfd[i] = connfd;
            Rio_readinitb(&p->clientrio[i], connfd);

            // 增加描述符到已准备的集合里面
            FD_SET(connfd, &p->read_set);

            // 更新最大描述符和池子的高水位标记
            if (connfd > p->maxfd)
                p->maxfd = connfd;
            if (i > p->maxi)
                p->maxi = i;
            break;
        }
    }

    // 如果找不到空闲槽位
    if (i == FD_SETSIZE)
        app_error("add_client error: Too many clients");
}

void check_clients(pool *p)
{
    int i, connfd, n;
    char buf[MAXLINE];
    rio_t rio;
    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++)
    {
        connfd = p->clientfd[i];
        rio = p->clientrio[i];
        // 如果描述符准备好了，echo一个文本行回去
        if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set)))
        {
            p->nready--;
            // 这里就是业务逻辑，即读取一行，就写回一行，实际上这里没有做到真正的事件驱动
            if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
            {
                byte_cnt += n;
                printf("Server recived %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
                Rio_writen(connfd, buf, n);
            }
            else
            {
                Close(connfd);
                FD_CLR(connfd, &p->read_set);
                p->clientfd[i] = -1;
            }
        }
    }
}