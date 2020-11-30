/*
 * @Author: your name
 * @Date: 2020-11-29 15:23:49
 * @LastEditTime: 2020-11-30 22:23:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\sbuf.h
 */
#ifndef _SBUF
#define _SBUF
#include "csapp.h"

typedef struct {
    int *buf;       //buffer队列
    int n;          //槽的最大值
    int front;      //buf[(front+1)%n]是头部
    int rear;       //buf[rear%n]是尾部
    sem_t mutex;    //保护对buf的访问
    sem_t slots;    //可用的slots计数
    sem_t items;    //可用的item计数
} sbuf_t;

void sbuf_init(sbuf_t *sp, int n);

// 清理buffer
void sbuf_deinit(sbuf_t *sp);

// 向buffer插入数据到buffer队尾
void sbuf_insert(sbuf_t *sp, int item);

// 移除并返回buffer队头的数据
int sbuf_remove(sbuf_t *sp);

#endif