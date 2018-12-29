# Designing a Thread Pool

## Question

Thread pools were introduced in Section 4.5.1. When thread pools are used, a task is submitted to the pool and executed by a thread from the pool. Work is submitted to the pool using a queue, and an available thread removes work from the queue. If there are no available threads, the work remains queued until one becomes available. If there is no work, threads await notification until a task becomes available. This project involves creating and managing a thread pool, and it may be completed using either Pthreds and POSIX synchronization or Java. Below we provide the details relevant to each specific technology. 

## Answer

按照题目中的要求，我采用了C语言去编写这次的程序。

pool_init()函数用于初始化线程池，用pthread_create()函数创建所需的线程，代码如下：

```c
void pool_init(int number)
{
    sem_init(&a,0,2);
    int i=0;
    for (i=0;i<number;i++){
        pthread_create(&bee,NULL,worker,NULL);
    }
}
```
pool_submit(void (*somefunction)(void *p), void *p)函数中的somefunction是指向将由池中的线程执行的函数的指针，p是传递给该函数的参数。这个函数通过调用en_queue()函数将任务添加到队列中，空闲的线程将使用dequeue()从任务队列中获取任务。这里使用了一个pthread_mutex_lock()函数，设置互斥锁，用于防止死锁的出现。代码如下：

```c
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
```

pool_shutdown()函数是用来停止线程池的。它通过调用phread_joint函数等待所有线程完成任务。代码如下：

```c
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
}
```

## 运行结果

我在客户端中采用了五个线程，Task如下：

```c
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
```

在ubuntu命令行运行结果如下：

<img src="C:\Users\lenovo\Desktop\1.png" style="zoom:80%" />




