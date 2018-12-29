# The Producer – Consumer Problem

## Question

In Section 7.1.1, we presented a semaphore-based solution to the producer– consumer problem using a bounded buffer. In this project, you will design a programming solution to the bounded-buffer problem using the producer and consumer processes shown in Figures 5.9 and 5.10. The solution presented in ection 7.1.1 uses three semaphores: empty and full, which count the number of empty and full slots in the buffer, and mutex, which is a binary (or mutualexclusion) semaphore that protects the actual insertion or removal of items in the buffer. For this project, you will use standard counting semaphores for empty and full and a mutex lock, rather than a binary semaphore, to represent mutex. The producer and consumer—running as separate threads —will move items to and from a buffer that is synchronized with the empty, full, and mutex structures. You can solve this problem using either Pthreads or the Windows API.

## Answer

buffer将包含一个固定大小的类型缓冲区项数组(将使用typedef定义该数组)。buffer item对象的数组将作为循环队列操作。buffer item的定义以及缓冲区的大小可以存储在头文件中。insert_item()和remove_item()函数将使用图7.1和图7.2中的算法来同步生产者和消费者。buffer还需要一个初始化函数来初始化互斥对象互斥锁以及空的和满的信号量。代码如下：

```c
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
```

生产者线程将在休眠一段随机时间和向缓冲区插入一个随机整数之间进行切换。将使用rand()函数生成随机数，该函数生成0到rand MAX之间的随机整数。消费者还将随机休眠一段时间，并在唤醒时尝试从buffer中删除项目。代码如下：

```c
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
```

## 运行结果

这个程序需要在命令行中调用时输入睡眠时间，生产者、消费者的数目，运行结果如下：

<img src="C:\Users\lenovo\Desktop\2.png" style="zoom:80%" />

