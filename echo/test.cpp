/*
 * @Author: your name
 * @Date: 2020-11-16 16:36:13
 * @LastEditTime: 2020-11-16 21:43:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\RIO\test.cpp
 */
#include "csapp.h"
#include <unistd.h>
// 展示如何使用RIO函数来一次一行地从标准输入复制一个文本文件到标准输出
int main(int argc, char **argv)
{
  int n;
  rio_t rio;
  char buf[MAXLINE];

  rio_readinitb(&rio, STDIN_FILENO);
  while((n = rio_readlineb(&rio, buf, MAXLINE))!=0)
    rio_writen(STDOUT_FILENO, buf, n);
}