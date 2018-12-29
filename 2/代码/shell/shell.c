#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#define out_redirect    1  
#define in_redirect     2  
#define have_pipe       3
char recent[200];
void getInput(char *);   
void instructionSegment(char *, int *, char a[ ][256]);
void executeCmd(int, char a[ ][256]);                  

int main(void)
{
    int should_run=1;
    int    i;
    int    argcount = 0;
    char   arglist[100][256];
    char   **arg = NULL;
    char   *buf  = NULL;
    buf = (char *)malloc(256);
    while(should_run) {
        
        printf("osh> ");
        fflush(stdout);
        memset(buf, 0, 256);
        getInput(buf);
        if( strcmp(buf,"exit\n") == 0)
            break;
        for (i=0; i < 100; i++)
        {
            arglist[i][0]='\0';
        }
        argcount = 0;
        instructionSegment(buf, &argcount, arglist);
        executeCmd(argcount, arglist);
    }
    if(buf != NULL) {
        free(buf);
        buf = NULL;
    }
    exit(0);
}


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


