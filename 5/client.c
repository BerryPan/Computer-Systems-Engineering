/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    struct data work[5];
    int i=0;
    for (i=0;i<5;i++){
        work[i].a = i;
        work[i].b = i+1;
    }
    pool_init(3);
    for (i=0;i<5;i++)
        pool_submit(&add,&work[i]);
    sleep(3);
    pool_shutdown();

    return 0;
}
