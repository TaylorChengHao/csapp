/*
 * @Author: your name
 * @Date: 2020-11-29 18:31:08
 * @LastEditTime: 2020-11-30 23:07:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\echo_cnt.cpp
 */
/* 
 * A thread-safe version of echo that counts the total number
 * of bytes received from clients.
 */

// 一个线程安全的echo业务逻辑
#include "csapp.h"

static int byte_cnt;  /* Byte counter */
static sem_t mutex;   /* and the mutex that protects it */

static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}

void echo_cnt(int connfd) 
{
    int n; 
    char buf[MAXLINE]; 
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    // 此函数只会被执行一次
    Pthread_once(&once, init_echo_cnt); //line:conc:pre:pthreadonce
    Rio_readinitb(&rio, connfd);        //line:conc:pre:rioinitb
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
	P(&mutex);
    // PV的中间是业务逻辑，要注意对byte_cnt这个线程共享数据加锁（PV）
	byte_cnt += n; //line:conc:pre:cntaccess1
	printf("server received %d (%d total) bytes on fd %d\n", 
	       n, byte_cnt, connfd); //line:conc:pre:cntaccess2
	V(&mutex);
	Rio_writen(connfd, buf, n);
    }
}
/* $end echo_cnt */

