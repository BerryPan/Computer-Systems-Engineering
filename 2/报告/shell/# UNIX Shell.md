# UNIX Shell

## 问题
​	This project consists of designing a C program to serve as a shell interface that accepts user commands and then executes each command in a separate process. Your implementation will support input and output redirection, as well as pipes as a form of IPC between a pair of commands. Completing this project will involve using the UNIX `fork()`, `exec()`, `wait()`, `dup2()`, and `pipe()` system calls and can be completed on any Linux, UNIX, or macOS system.
​	This project is organized into several parts:
1. Creating the child process and executing the command in the child
2. Providing a history feature
3. Adding support of input and output redirection
4. Allowing the parent and child processes to communicate via a pipe

## 解决方案
### 读取命令
首先定义一个`getInput`函数，用于读取shell中的指令，并将指令存入名为`buffer`的变量中。此外，在这个函数中判断输入是否为`!!`，如果是`!!`，则用`recent`存入`buffer`，用于后面的输出历史指令，如果不是，则将读取的数据存入`buffer`，同时存入`recent`。`recent`是一个全局变量，用于记录上一次输入的命令。这样就解决了输入`!!`来执行上一条命令的要求。代码如下：
```c
void getInput(char *buf)
{
    int len = 0;
    int ch;
    ch = getchar();
    while (len < 256 && ch != '\n') {
        buf[len++] = ch;
        ch = getchar();
    }

    if(len == 256) {
        printf("command is too long \n");
        exit(-1); 
    }
    if(buf[0]=='!'&buf[1]=='!'){
    	memset(buf,0,sizeof(buf));
        for(int i=0; i<strlen(recent); i++)
            buf[i]=recent[i];
        int i = strlen(recent);        
        buf[i] = '\n';
        i++;
        buf[i] = '\0';
    }
    else{
    	memset(recent,0,sizeof(recent));
        for(int i=0; i<strlen(buf); i++)
            recent[i]=buf[i];
        buf[len] = '\n';
        len++;
        buf[len] = '\0';
    }
}
```

### 对读取到的指令进行分解
在这一步定义了名为`instructionSegment`的函数，对buffer按照空格和`‘\n’`进行分解，来装载`arglist`，代码如下：
```c
void instructionSegment(char *buf, int *argcount, char arglist[100][256])
{
    char    *p  = buf;
    char    *q;
    int number  = 0;
    while (1) {
        if ( p[0] == '\n' )
            break;

        if ( p[0] == ' '  )
            p++;
        else {
            q = p;
            number = 0;
            while( (q[0]!=' ') && (q[0]!='\n') ) {
                number++;
                q++;
            }
            strncpy(arglist[*argcount], p, number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}
```

### 执行arglist中的命令
定义`executeCmd`函数，用于执行`arglist`中的命令，需要注意的是，问题中的三个要求都将在这个函数中实现。
首先是判断arglist中是否存在`<`、`>`、`|`，如果存在，则将变量`how`赋为相应的值。
然后对how分情况进行操作，如果是`<`、`>`，则将其后面的一个变量当作文件名，如果是`|`，则将后面的命令复制到`argnext`中。
最后一步是执行操作。首先用`fork()`创建一个子进程，然后对hoW进行switch判断。
1. 如果how是0，即基本情况，则直接利用`execvp(arg[0], arg)`执行命令；
2. 如果how是1，即是`<`，则首先用open函数打开文件，open的函数原型为
```c
int open(const char *path, int oflags,mode_t mode);
```
在这里，`oflags`采用O_RDWR(可读可写)，O_CREAT(如果指定文件不存在，则创建这个文件)，O_TRUNC(如果文件存在，并且以只写/读写方式打开，则清空文件全部内容)，`mode`采用0644，只有在第二个参数中有O_CREAT的情况下才能使用，表示open文件访问权限的初始值，0644表示用户具有读写权限，组用户和其它用户具有只读权限。
然后使用dup2函数，用来复制一个现有的文件描述符，函数原型为
```c
int dup2( int filedes, int filedes2 ) 
```
我们调用dup2函数，参数为fd和1，这会导致用我们新打开的文件描述符替换掉由1代表的文件描述符（即stdout，因为标准输出文件的id为1）。需要注意的是，任何写到stdout的东西，现在都将改为写入上一步打开的文件中。需要注意的是，dup2函数在复制了fd之后，会立即将其关闭，但不会关掉新近打开的文件描述符，因为文件描述符1现在也指向它。
最后用execvp函数执行即可。

3. 如果how是1，即是`>`，则首先用open函数打开文件，和2不同的是，这里打开的是已经存在的文件，且只需要读取不用写入，oflags采用O_RDONLY(只读)，mode则不用输入。然后调用dup2函数，参数为fd和0，这会导致用我们新打开的文件描述符替换掉由0代表的文件描述符（即stdin，因为标准输入文件的id为0）。最后用execvp函数执行即可。
4. 如果how是3，即是`|`，则需要再创建一个子进程，同时用2的操作，在该子进程中执行第一段命令，并将输出结果写入到自己创建的一个文件中，之后再使用3的操作，将输入流改成上一步创建的文件，并在子进程中执行第二段命令。
代码如下：
```c
void executeCmd(int argcount, char arglist[100][256])
{
    int flag = 0;
    int how = 0;       
    int background = 0; 
    int status;
    int i;
    int fd;
    char*   arg[argcount+1];
    char*   argnext[argcount+1];
    char*   file;
    pid_t   pid;

    for (i=0; i < argcount; i++) {
        arg[i] = (char *) arglist[i];
    }
    arg[argcount] = NULL;

    for (i=0; i < argcount; i++) {
        if (strncmp(arg[i], "&",1) == 0) {
            if (i == argcount-1) {
                background = 1;
                arg[argcount-1] = NULL;
                break;
            }
            else {
                printf("wrong command\n");
                return ;
            }
        }
    }

    for (i=0; arg[i]!=NULL; i++) {
        if (strcmp(arg[i], ">") == 0 ) {
            flag++;
            how = out_redirect;
            if (arg[i+1] == NULL)
                flag++;
        }
        if ( strcmp(arg[i],"<") == 0 ) {
            flag++;
            how = in_redirect;
            if(i == 0)
                flag++;
        }
        if ( strcmp(arg[i],"|")==0 ) {
            flag++;
            how = have_pipe;
            if(arg[i+1] == NULL)
                flag++;
            if(i == 0 )
                flag++;
        }
    }
    if (flag > 1) {
        printf("wrong command\n");
        return;
    }
    if (how == out_redirect) {
        for (i=0; arg[i] != NULL; i++) {
            if (strcmp(arg[i],">")==0) {
                file   = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if (how == in_redirect) {   
        for (i=0; arg[i] != NULL; i++) {
            if (strcmp (arg[i],"<") == 0) {
                file   = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if (how == have_pipe) {
        for (i=0; arg[i] != NULL; i++) {
            if (strcmp(arg[i],"|")==0) {
                arg[i] = NULL;
                int j;
                for (j=i+1; arg[j] != NULL; j++) {
                    argnext[j-i-1] = arg[j];
                }
                argnext[j-i-1] = arg[j];
                break;
            }
        }
    }
    if ( (pid = fork()) < 0 ) {
        printf("fork error\n");
        return;
    }

    switch(how) {
        case 0:
            if (pid == 0) {
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 1:
            if (pid == 0) {
                fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
                dup2(fd,1);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 2:
            if (pid == 0) {
                fd = open(file,O_RDONLY);
                dup2(fd,0);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 3:
            if(pid == 0) {
                pid_t  pid2;
                int  status2;
                int  fd2;
                if ((pid2=fork())==0) {
                    fd2 = open("/tmp/pjm",
                               O_RDWR|O_CREAT|O_TRUNC,0644);
                    dup2(fd2, 1);
                    execvp(arg[0], arg);
                    exit(0);
                }
                if (waitpid(pid2, &status2, 0) == -1)
                    printf("wait for child process error\n");
                fd2 = open("/tmp/pjm",O_RDONLY);
                dup2(fd2,0);
                execvp (argnext[0],argnext);
                if ( remove("/tmp/pjm") )
                    printf("remove error\n");
                exit(0);
            }
            break;
        default:
            break;
    }

    if ( background == 1 ) {
        printf("[process id %d]\n",pid);
        return ;
    }

    if (waitpid (pid, &status,0) == -1)
        printf("wait for child process error\n");
}
```

## 执行结果
### 历史记录
如图，输入!!可以直接执行上一步的操作

<img src="C:\Users\lenovo\Desktop\4.jpg" style="zoom:80%" />

### 文件输出
<img src="C:\Users\lenovo\Desktop\1.jpg" style="zoom:80%" />

打开out.txt可以看到，ls已经输入到out.txt中了。

<img src="C:\Users\lenovo\Desktop\2.jpg" style="zoom:80%" />

### 文件输入

如图，in.txt的数据已经用sort按照从小到大排序了

<img src="C:\Users\lenovo\Desktop\3.jpg" style="zoom:80%" />

### 管道

如图，输入ls -l | less，进入如下界面

<img src="C:\Users\lenovo\Desktop\5.jpg" style="zoom:80%" />

按q退出，可以看到命令行

<img src="C:\Users\lenovo\Desktop\6.jpg" style="zoom:80%" />