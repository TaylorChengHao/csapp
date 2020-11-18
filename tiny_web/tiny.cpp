/*
 * @Author: your name
 * @Date: 2020-11-17 16:49:58
 * @LastEditTime: 2020-11-18 16:26:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\tiny_web\tiny.cpp
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void server_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void server_dynamic(int fd, char *fielname, char *cgiargs);
void clienterror(const int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    // 检测参数
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr,clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Connect to (%s, %s)\n", hostname, port);
        // 业务逻辑
        doit(connfd);
        Close(connfd);
    }
}

void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    // 读取请求行和头
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    printf("Request headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    // 只实现了GET请求
    if (strcasecmp(method, "GET"))
    {
        clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
        return;
    }
    read_requesthdrs(&rio);

    // 处理uri
    is_static = parse_uri(uri, filename, cgiargs);
    
    printf("doit uri:%s,filename:%s,cgiargs:%s\n", uri, filename, cgiargs);
    if (stat(filename, &sbuf) < 0)
    {
        clienterror(fd, filename, "404", "Not found", "Tiny could't find this file");
        return;
    }
    
    if (is_static)
    {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
        {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
            return;
        }
        server_static(fd, filename, sbuf.st_size);
    }else
    {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
            return;
        }
        server_dynamic(fd, filename, cgiargs);
    }
    
}

// 发送一个http响应到客户端,报告错误
void clienterror(const int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    // 构造http response的body
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em> The Tiny Web server\r\n", body);

    // Print the HTTP response
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type:text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

// 读取并忽略请求报头
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}

// Tiny假设静态内容的主目录就是它的当前目录，而可以执行的主目录是./cgi-bin。
// 任何包含“cgi-bin”的uri都会被认为是对动态内容的请求，默认的文件名是./home.html
int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin"))            //静态内容
    {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if (uri[strlen(uri)-1] == '/')
            strcat(filename, "home.html");
        return 1;
    }
    else
    {
        // 动态内容
        ptr = index(uri, '?');
        if (ptr)
        {
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, ".");
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

// TINY提供5种常见的静态内容，html文件、无格式的文本文件，以及编码为GIF、PNG和JPG格式的文件。
void server_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    // 发送response header给客户端
    get_filetype(filename,filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer:Tiny WebServer\r\n", buf);
    sprintf(buf, "%sConnection:close\r\n", buf);
    sprintf(buf, "%sContent-length:%d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type:%s\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));
    printf("Response header:\n");
    printf("%s", buf);

    // 发送response body给客户端
    srcfd = Open(filename, O_RDONLY, 0);
    // mmap函数将文件映射到内存
    srcp = (char *)Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    printf("srcp:%s\n",srcp);
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize);
}

void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpg");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpg");
    else
        strcpy(filetype, "text/plain");
}

// TINY通过派生一个子进程并在子进程的上下文中运行一个cgi程序
void server_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = {NULL};

    // 返回HTTP response的第一部分
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server:Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0)        //Child
    {
        // 这里简化了，参数只有1个
        printf("server_dynamic cgiargs:%s\n", cgiargs);
        printf("filename :%s\n",filename);
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);        //Redirect stdout to client
        Execve(filename, emptylist, environ);       //Run CGI program
    }
    Wait(NULL);             //Parent waits for reaps child
}







