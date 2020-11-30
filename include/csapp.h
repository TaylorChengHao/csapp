/*
 * @Author: your name
 * @Date: 2020-11-16 21:39:34
 * @LastEditTime: 2020-11-30 21:44:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\include\csapp.h
 */
#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void unix_error(char *msg);
void gai_error(int code, char *msg);
void app_error(const char *msg);
void posix_error(int code, char *msg);

#define RIO_BUFSIZE 8192
#define MAXLINE 1024
#define LISTENQ 1024 //指定未完成连接队列的最大长度.如果一个连接请求到达时未完成连接队列已满,那么客户端将接收到错误
#define MAXBUF 8192  //IO buffer的最大值

typedef struct
{
    int rio_fd;                //文件描述符
    int rio_cnt;               //未读字节数
    char *rio_bufptr;          //下一个未读字节
    char rio_buf[RIO_BUFSIZE]; //buffer
} rio_t;
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *usrbuf, size_t n);
void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);
void Free(void *ptr);

typedef struct sockaddr SA;
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res);
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);
void Freeaddrinfo(struct addrinfo *res);
int open_clientfd(char *hostname, char *port);
int Open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);
int Open_listenfd(char *port);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
char *Fgets(char *ptr, int n, FILE *stream);
FILE *Fopen(const char *filename, const char *mode);
void Fputs(const char *ptr, FILE *stream);
void Close(int fd);
int Open(const char *pathname, int flags, mode_t mode);

void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(void *start, size_t length);

pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
int Dup2(int fd1, int fd2);

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, 
		    void * (*routine)(void *), void *argp);
void Pthread_detach(pthread_t tid);
void Pthread_once(pthread_once_t *once_control, void (*init_function)());

void Sem_init(sem_t *sem, int pshared, unsigned int value);
void P(sem_t *sem);
void V(sem_t *sem);

#endif