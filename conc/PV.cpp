/*
 * @Author: your name
 * @Date: 2020-11-29 18:04:51
 * @LastEditTime: 2020-11-29 18:05:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\conc\PV.cpp
 */

#include "csapp.h"

// 第一类的读者-写者问题，读者优先
int readcnt;
sem_t mutex, w, wg;
void reader(void)
{
    while (1)
    {
        P(&mutex);
        readcnt++;
        if(readcnt == 1)    //第一个读者，此时就不让写了
            P(&w);
        V(&mutex);

        // 读取的代码的位置
        // 

        P(&mutex);
        readcnt--;
        if (readcnt == 0)   //最后一个读者，让写
            V(&w);
        V(&mutex);
    }
}
void writer(void)
{
    while (1)
    {
        P(&wg);
        P(&w);
        
        // 写的代码的位置
        // 

        V(&w);    
        V(&wg);
    }
}

// 第二类的读者-写者问题，写者优先
int readcnt, writecnt;
sem_t mutex, w, wg, r;
void reader(void)
{
    while (1)
    {
        P(&r);
        P(&mutex);
        readcnt++;
        if(readcnt == 1)    //第一个读者，此时就不让写了
            P(&w);
        V(&mutex);

        // 读取的代码的位置
        // 

        P(&mutex);
        readcnt--;
        if (readcnt == 0)   //最后一个读者，让写
            V(&w);
        V(&mutex);
        V(&r);
    }
}
void writer(void)
{
    while (1)
    {
        P(&mutex);
        // 排队计数
        writecnt++;
        // 一旦有排队（排队为1指有一个写者正在执行），不让读者执行
        if (writecnt == 1)
            P(&r);
        V(&mutex);
        P(&wg);
        P(&w);
        
        // 写的代码的位置
        // 

        V(&w);    
        V(&wg);
        P(&mutex);
        writecnt--;
        if (writecnt == 0)
            V(&r);
        V(&mutex);
    }
}