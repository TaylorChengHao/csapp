/*
 * @Author: your name
 * @Date: 2020-11-29 15:32:11
 * @LastEditTime: 2020-11-30 22:37:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\sbuf.cpp
 */
#include "sbuf.h"

// 为缓冲区分配堆内存
void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = (int *)Calloc(n, sizeof(int));
    sp->n = n;
    sp->front = sp->rear = 0;
    Sem_init(&sp->mutex, 0, 1);         // 用于当锁的二元信号量
    Sem_init(&sp->slots, 0, n);         // buf有了n个空slots（作为资源的信号量）
    Sem_init(&sp->items, 0, 0);         // buf有0个数据  
}

// 清理buffer
void sbuf_deinit(sbuf_t *sp)
{
    Free(sp->buf);
}

// 向buffer插入数据到buffer队尾
void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);                      // 等待一个可用的slot
    P(&sp->mutex);                      // 加锁
    sp->buf[(++sp->rear)%(sp->n)] = item;   // 放入item
    V(&sp->mutex);                      // unlock
    V(&sp->items);                      // 宣布item可得
}

// 移除并返回buffer队头的数据
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);                      // 等待可用的数据item
    P(&sp->mutex);                      // 锁上buffer
    item = sp->buf[(++sp->front)%(sp->n)];          // 移除数据
    V(&sp->mutex);                      // unlock
    V(&sp->slots);                      // 宣布slot可用
    return item;
}