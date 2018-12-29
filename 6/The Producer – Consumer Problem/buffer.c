#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define random(x) (rand()%x)

#include <pthread.h>
#include <semaphore.h>

#include "buffer.h"

# define true 1

buffer_item buffer[BUFFER_SIZE];   // bounded buffer

pthread_mutex_t mutex;  // lock
sem_t empty;  // semaphore representing the size of buffer
sem_t full;

// pthread id
pthread_t bee1, bee2;

// index for insert and remove
int index_insert = 0;
int index_remove = 0;

int insert_item(buffer_item item)
{
    buffer[index_insert] = item;
    index_insert = (index_insert + 1) % BUFFER_SIZE;
    return 0;
}

int remove_item(buffer_item *item)
{
    *item = buffer[index_remove];
    index_remove = (index_remove + 1) % BUFFER_SIZE;
    return 0;
}

void *producer(void *param)
{
    buffer_item item;

    while (true){
        srand((int)time(0));
        sleep(random(5));
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // critical section
        srand((int)time(0));
        item = random(10);
        insert_item(item);
        printf("producer produced %d\n",item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

void *consumer(void *param)
{
    buffer_item item;

    while(true){
        srand((int)time(0));
        sleep(random(5));
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // critical section
        remove_item(&item);
        printf("consumer consumed %d\n",item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

int main(int argc, char *argv[])
{
    int sleep_time, producer_number, consumer_number;

    sleep_time = atoi(argv[1]);
    producer_number = atoi(argv[2]);
    consumer_number = atoi(argv[3]);

    pthread_mutex_init(&mutex, NULL);
    sem_init(&full,0,BUFFER_SIZE);
    sem_init(&empty,0,0);

    // invoke producer
    int i=0;
    for (i=0;i<producer_number;i++)
        pthread_create(&bee1,NULL,&producer,NULL);

    // invoke consumer
    int j=0;
    for (j=0;j<consumer_number;j++)
        pthread_create(&bee2,NULL,&consumer,NULL);

    // sleep for given time
    sleep(sleep_time);

    return 0;
}
