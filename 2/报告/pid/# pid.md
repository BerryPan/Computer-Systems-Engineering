# pid
## 问题
In this project, you will write a Linux kernel module that uses the `/proc` file system for displaying a task’s information based on its process identifier value pid. Before beginning this project, be sure you have completed the Linux kernel module programming project in Chapter 2, which involves creating an entry in the `/proc` file system. This project will involvewriting a process identifier to the file `/proc/pid`. Once a pid has been written to the `/proc` file, subsequent reads from /proc/pid will report (1) the command the task is running, (2) the value of the task’s pid, and (3) the current state of the task. An example of how your kernel module will be accessed once loaded into the system is as follows:
```shell
echo "1395" > /proc/pid
cat /proc/pid
command = [bash] pid = [1395] state = [1]
```
The echo command writes the characters "1395" to the `/proc/pid` file. Your kernel module will read this value and store its integer equivalent as it represents a process identifier. The cat command reads from `/proc/pid`, where your kernel module will retrieve the three fields from the task struct associated with the task whose pid value is 1395.

## 解决方案
和project1相似，只需要在project1的基础上加上`proc_write`函数即可。
首先在开头定义函数`proc_write`，函数原型为
```c
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);
```
然后在结构体`proc_ops`中，需要加入
```c
.write = proc_write,
```
这样使得`/proc/pid`发生写操作时，会调用`proc_write()`函数。

之后定义`proc_write`函数，其中调用的`kmalloc()`函数是用来分配内存的函数，函数原型如下：
```c
Void *kmalloc(size_t size, int flags);
```
第一个参数是要分配的块的大小，第二个参数是分配标志（flags），他提供了多种kmalloc的行为。这里采用`GFP_KERNEL`表示内存分配（最终总是调用get_free_pages来实现实际的分配，这就是GFP前缀的由来）是代表运行在内核空间的进程执行的。使用GFP_KERNEL容许kmalloc在分配空闲内存时候如果内存不足容许把当前进程睡眠以等待。因此这时分配函数必须是可重入的。

copy_from_user()函数将usr_buf的内容(包含写入/proc/pid的内容)复制到最近分配的内核内存中。

`kstrtol()`是将一个字符串转换成无符号整型，函数原型如下：
```c
int kstrtol (const char * s, unsigned int base, long * res);
```
当给出一个pid时，`pid_task()`函数将返回对应的`task_struct`，也就是linux的PCB，函数原型如下：
```c
struct task_struct pid_task(struct pid *pid, enum pid type type)
```
proc_write的代码如下：
```c
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    char *k_mem;
    char mem[100];
    k_mem = kmalloc(count, GFP_KERNEL);
    if (copy_from_user(k_mem, usr_buf, count)) {
        printk( KERN_INFO "Error copying from user\n");
        return -1;
    }

    printk(KERN_INFO "K_MEM-> %s\n", k_mem);
    sscanf(k_mem,"%ld\n%s", &l_pid, mem);

    struct task_struct *tsk = NULL;
    if (pid_task(find_vpid(l_pid), PIDTYPE_PID)){
    	tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);
	strcpy(command, tsk->comm);
	pid_state = tsk->state;
    }else{
    	printk("no correct pid found");
    }

    kfree(k_mem);
    return count;
}
```

### 运行结果
<img src="C:\Users\lenovo\Desktop\1.jpg" style="zoom:80%" />