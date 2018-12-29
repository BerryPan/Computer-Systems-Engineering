
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// semaphore variable
static sem_t a;

// work to be completed by thread in pool
typedef struct
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo[QUEUE_SIZE];
 //the index for work queue
int index=0;

// the worker bee(pthread id)
pthread_t bee;

// insert a task into the queue
// returns 0 if successful or 1 otherwise
int enqueue(task t)
{
    if (index<10){
        worktodo[index] = t;
        index += 1;
        return 0;
    }
    return 1;
}

// remove a task from the queue
task dequeue()
{
    task work = worktodo[0];
    if (index>0){
        int i;
        for (i=0;i<9;i++)
            worktodo[i] = worktodo[i+1];
        index -= 1;
        return work;
    }
    else{
        pthread_exit(0);
        return work;
    }
}

// the worker thread in the thread pool
void *worker(void *param)
{
    while (index>0){
        sem_wait(&a);
        pthread_mutex_lock(&mutex);

        task work = dequeue();
        // execute the task
        execute(work.function, work.data);

        pthread_mutex_unlock(&mutex);
        sem_post(&a);
    }
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
 void execute(void (*somefunction)(void *p), void *p)
 {
     (*somefunction)(p);
 }

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task new_work;
    new_work.function = somefunction;
    new_work.data = p;

    pthread_mutex_lock(&mutex);
    int result = enqueue(new_work);
    pthread_mutex_unlock(&mutex);

    if (result==1){
        printf("Queue full!\n");
    }

    return 0;
}

// initialize the thread pool
void pool_init(int number)
{
    sem_init(&a,0,2);
    int i=0;
    for (i=0;i<number;i++){
        pthread_create(&bee,NULL,worker,NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
}
