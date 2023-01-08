#include<stdio.h>
#include<sys/wait.h>
#include<sys/unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/file.h>
#include<sys/fcntl.h>
#include<signal.h>
int checkSimple(const char *);
void executeSimple(const char *);
void HandleStop();
void removeNewLine(char **,int);
int CheckchangeDir(const char *);
void executeChangeDir(const char *);
int CheckExit(const char *);
void executeExit();
int CheckGetLogin(const char *);
void executeGetLogin();
int CheckFG(const char *);
void executeFG();
int checkOnlysigns(const char *);
void executeOnlySigns(const char *);
int checkOnlyPipes(const char *);
void executeOnlyPipes(const char *);
void removeQuotes(char *);
int checkBothSignsAndPipes(const char *);
void executeBothSignsAndPipes(const char *);
struct stack{
int top;
int * stackArray;
};
struct stack * createStack(int capacity)
{
    struct stack * stack=(struct stack *)malloc(sizeof(struct stack));
    stack->top=-1;
    stack->stackArray=(int*)malloc(capacity*sizeof(int));
    return stack;
}
void Enqueue(struct stack *stack, int num)
{
    if(stack->top<100)
    {
        stack->stackArray[++stack->top]=num;
    }
    return;
}
int Dequeue(struct stack * stack)
{
    if(stack->top>=0)
    {
        int num=stack->stackArray[stack->top];
        stack->top--;
        return num;
    }
    return 0;
}
int isEmpty(struct stack * stack)
{
    if(stack->top==-1)
    {
        return 1;
    }
    return 0;
}
void printStack(struct stack * stack)
{
    if(isEmpty(stack)==0)
    {
        printf("\nStack values : ");
        for(int i=0;i<=stack->top;i++)
        {
            printf("%d ",stack->stackArray[i]);
        }
    }
    else
    {
        printf("\nstack is empty!\n");
    }
}
struct stack * JobStack=NULL;
int child=0,parent=0;
int main(int argc,char ** argv)
{
    JobStack=createStack(100);
    while(1)
    {
        printf("\n\n\t\t\t\t\t C SHELL COMMAND INTERPRETER \U0001F607 : \n\n");
        size_t size=100;
        char * command = (char *) malloc(size*sizeof(char));
        printf("%s: ",getcwd(command,size));
        getline(&command,&size,stdin);
        removeQuotes(command);
        printf("\n");
        if(CheckExit(command)==1)
        {
            executeExit();
        }
        else if(CheckchangeDir(command)==1)
        {
            executeChangeDir(command);
        }
        else if(CheckGetLogin(command)==1)
        {
            executeGetLogin();
        }
        else if(CheckFG(command)==1)
        {
            executeFG();
        }
        else if(checkSimple(command)==1)
        {
            executeSimple(command);
        }
        else if(checkOnlysigns(command)==1)
        {
            executeOnlySigns(command);
        }
        else if(checkOnlyPipes(command)==1)
        {
            executeOnlyPipes(command);
        }
        else if(checkBothSignsAndPipes(command)==1)
        {
            executeBothSignsAndPipes(command);
        }
        else
        {
            printf("\nCommand Not Found!\n");
        }
        free(command);
        command=NULL;
        printf("\nPress enter to continue...");
        getchar();
        system("clear");
    }
    return 0;
}

int CheckchangeDir(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    for(int i=0;token[i]!='\0';i++)
    {
        if(token[i]=='\n')
        {
            for(int j=i;token[j]!='\0';j++)
            {
                token[j]=token[j+1];
            }
        }
    }
    if(strcmp(token,"cd")==0)
    {
        free(copy);
        return 1;
    }
    free(copy);
    return 0;
}
void executeChangeDir(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    int size=0;
    char ** token = (char **) malloc (10*sizeof(char*));
    token[0]=strtok(copy," ");
    while(token[size]!=NULL)
    {
        token[++size]=strtok(NULL," ");
    }
    size++;
    token = (char **) realloc(token,size*sizeof(char *));
    removeNewLine(token,size);
    pid_t pid=fork();
    if(pid==0)
    {
        if(size<=2)
        {
            chdir("..");
        }
        else
        {
            size_t size=100;
            char dir[100];
            sprintf(dir,"%s/%s",getcwd(dir,size),token[1]);
            if(chdir(dir)==-1)
            {
                printf("\n%s Directory not Found!\n",token[1]);
            }
        }
    }
    else if(pid>0)
    {
        waitpid(pid,NULL,0);
    }
    else
    {
        printf("Fork Failed!");
        exit(-1);
    }
    free(token);
    free(copy);
    return;
}
int CheckExit(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    for(int i=0;token[i]!='\0';i++)
    {
        if(token[i]=='\n')
        {
            for(int j=i;token[j]!='\0';j++)
            {
                token[j]=token[j+1];
            }
        }
    }
    if(strcmp(token,"exit")==0)
    {
        free(copy);
        return 1;
    }
    free(copy);
    return 0;
}
void executeExit()
{
    printf("\nThank You! For Using The C SHELL COMMAND INTERPRETER.\n\n");
    exit(EXIT_SUCCESS);
    return;
}
int CheckGetLogin(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    for(int i=0;token[i]!='\0';i++)
    {
        if(token[i]=='\n')
        {
            for(int j=i;token[j]!='\0';j++)
            {
                token[j]=token[j+1];
            }
        }
    }
    if(strcmp(token,"getlogin()")==0)
    {
        free(copy);
        return 1;
    }
    free(copy);
    return 0;
}
void executeGetLogin()
{
    printf("Logged In User : %s\n\n",getlogin());
    return;
}
int CheckFG(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    for(int i=0;token[i]!='\0';i++)
    {
        if(token[i]=='\n')
        {
            for(int j=i;token[j]!='\0';j++)
            {
                token[j]=token[j+1];
            }
        }
    }
    if(strcmp(token,"fg")==0)
    {
        free(copy);
        return 1;
    }
    free(copy);
    return 0;
}
void executeFG()
{
    if(isEmpty(JobStack)==0)
    {
        int job=Dequeue(JobStack);
        kill(job,SIGCONT);
        waitpid(job,NULL,0);
    }
    else
    {
        printf("\ncurrent: no such job\n");
    }
    return;
}
int checkSimple(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    while(token!=NULL)
    {
        if((strcmp(token,"|")==0) || (strcmp(token,">")==0) || (strcmp(token,"<")==0) || (strcmp(token,">>")==0))
        {
            free(copy);
            return 0;
        }
        token=strtok(NULL," ");
    }
    free(copy);
    return 1;
}
void HandleStop()
{
    signal(SIGTSTP,SIG_DFL);
    kill(child,SIGTSTP);
    printf("\nProcess Stopped with pid = %d\n\n",child);
    kill(parent,SIGCHLD);
    Enqueue(JobStack,child);
    return;
}
void executeSimple(const char * command)
{
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    int size=0;
    char ** token = (char **) malloc (10*sizeof(char*));
    token[0]=strtok(copy," ");
    while(token[size]!=NULL)
    {
        token[++size]=strtok(NULL," ");
    }
    size++;
    token = (char **) realloc(token,size*sizeof(char *));
    removeNewLine(token,size);
    signal(SIGTSTP,HandleStop);
    parent=getpid();
    pid_t pid=fork();
    if(pid==0)
    {
        if(strcmp(token[size-2],"&")==0)
        {
            token[size-2]=NULL;
        }
        if(execvp(token[0],token)<0)
        {
            printf("\nCommand Not Found!\n");
        }
    }
    else if(pid>0)
    {
        if(strcmp(token[size-2],"&")!=0)
        {
           child=pid;
           waitpid(pid,NULL,WUNTRACED);
        }
        else
        {
            Enqueue(JobStack,pid);
        }
    }
    else
    {
        printf("Fork Failed!");
        exit(-1);
    }
    free(token);
    free(copy);
    return;
}
int checkOnlysigns(const char * command)
{
    int check=0;
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    while(token!=NULL)
    {
        if((strcmp(token,"|")==0))
        {
            free(copy);
            return 0;
        }
        else if((strcmp(token,">")==0) || (strcmp(token,"<")==0) || (strcmp(token,">>")==0))
        {
            check++;
        }
        token=strtok(NULL," ");
    }
    free(copy);
    if(check==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void executeOnlySigns(const char * command)
{
    int less=0,great=0,app=0;
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    int size=0;
    char ** token = (char **) malloc (10*sizeof(char*));
    token[0]=strtok(copy," ");
    while(token[size]!=NULL)
    {
        token[++size]=strtok(NULL," ");
    }
    size++;
    token = (char **) realloc(token,size*sizeof(char *));
    removeNewLine(token,size);
    for(int i=0;i<size;i++)
    {
        if(token[i]!=NULL)
        {
            if(strcmp(token[i],">")==0)
            {
                great=i;
            }
            if(strcmp(token[i],"<")==0)
            {
                less=i;
            }
            if(strcmp(token[i],">>")==0)
            {
                app=i;
            }
        }
    }
    if(less!=0 && great==0 && app==0)
    {
        int index=0;
        for(int i=0;i<size;i++)
        {
            if(strcmp(token[i],"<")==0)
            {
                index=i;
                break;
            }
        }
        char * temp = (char *)malloc(100*sizeof(char));
        strcpy(temp,token[less+1]);
        for(int j=0;j<2;j++)
        {
            for(int i=index;i<size-1;i++)
            {
                token[i]=token[i+1];
            }
        }
        size=size-2;
        token = (char **) realloc(token,(size)*sizeof(char *));
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fd=open(temp,O_CREAT | O_APPEND | O_RDONLY,0777);
            dup2(fd,0);
            close(fd);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(token[0],token);
        }
        else if(pid>0)
        {
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        free(temp);
    }
    else if(less==0 && great!=0 && app==0)
    {
        int index=0;
        for(int i=0;i<size;i++)
        {
            if(strcmp(token[i],">")==0)
            {
                index=i;
                break;
            }
        }
        char * temp = (char *)malloc(100*sizeof(char));
        strcpy(temp,token[great+1]);
        for(int j=0;j<2;j++)
        {
            for(int i=index;i<size-1;i++)
            {
                token[i]=token[i+1];
            }
        }
        size=size-2;
        token = (char **) realloc(token,(size)*sizeof(char *));
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fd=open(temp,O_CREAT | O_TRUNC | O_RDWR,0777);
            dup2(fd,1);
            close(fd);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(token[0],token);
        }
        else if(pid>0)
        {
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        free(temp);
    }
    else if(less==0 && great==0 && app!=0)
    {
        int index=0;
        for(int i=0;i<size;i++)
        {
            if(strcmp(token[i],">>")==0)
            {
                index=i;
                break;
            }
        }
        char * temp = (char *)malloc(100*sizeof(char));
        strcpy(temp,token[app+1]);
        for(int j=0;j<2;j++)
        {
            for(int i=index;i<size-1;i++)
            {
                token[i]=token[i+1];
            }
        }
        size=size-2;
        token = (char **) realloc(token,(size)*sizeof(char *));
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fd=open(temp,O_CREAT | O_APPEND | O_RDWR,0777);
            dup2(fd,1);
            close(fd);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(token[0],token);
        }
        else if(pid>0)
        {
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        free(temp);
    }
    else if(less!=0 && great!=0 && app==0)
    {
        char * temp=(char *) malloc (100*sizeof(char));
        strcpy(temp,token[great+1]);
        char * temp2=(char *) malloc (100*sizeof(char));
        strcpy(temp2,token[less+1]);
        while(token[less]!=NULL)
        {
            for(int i=less;i<size-1;i++)
            {
                token[i] = token[i+1];
            }
            size--;
        }
        token = (char **) realloc(token,(size)*sizeof(char *));
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fdWrite=open(temp,O_CREAT | O_TRUNC | O_RDWR,0777);
            dup2(fdWrite,1);
            close(fdWrite);
            int fdRead=open(temp2,O_CREAT | O_APPEND | O_RDONLY,0777);
            dup2(fdRead,0);
            close(fdRead);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(token[0],token);
        }
        else if(pid>0)
        {
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        free(temp);
        free(temp2);
    }
    else if(less!=0 && great==0 && app!=0)
    {
        char * temp=(char *) malloc (100*sizeof(char));
        strcpy(temp,token[app+1]);
        char * temp2=(char *) malloc (100*sizeof(char));
        strcpy(temp2,token[less+1]);
        while(token[less]!=NULL)
        {
            for(int i=less;i<size-1;i++)
            {
                token[i] = token[i+1];
            }
            size--;
        }
        token = (char **) realloc(token,(size)*sizeof(char *));
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fdWrite=open(temp,O_CREAT | O_APPEND | O_RDWR,0777);
            dup2(fdWrite,1);
            close(fdWrite);
            int fdRead=open(temp2,O_CREAT | O_APPEND | O_RDONLY,0777);
            dup2(fdRead,0);
            close(fdRead);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(token[0],token);
        }
        else if(pid>0)
        {
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        free(temp);
        free(temp2);
    }
    free(token);
    free(copy);
    return;
}
int checkOnlyPipes(const char * command)
{
    int check=0;
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    while(token!=NULL)
    {
        if((strcmp(token,"|")==0))
        {
            check++;
        }
        else if((strcmp(token,">")==0) || (strcmp(token,"<")==0) || (strcmp(token,">>")==0))
        {
            free(copy);
            return 0;
        }
        token=strtok(NULL," ");
    }
    free(copy);
    if(check==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void executeOnlyPipes(const char * command)
{
    int pipes=0;
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    int size=0;
    char ** token = (char **) malloc (10*sizeof(char*));
    token[0]=strtok(copy," ");
    while(token[size]!=NULL)
    {
        token[++size]=strtok(NULL," ");
    }
    size++;
    token = (char **) realloc(token,size*sizeof(char *));
    removeNewLine(token,size);
    for(int i=0;i<size;i++)
    {
        if(token[i]!=NULL)
        {
            if(strcmp(token[i],"|")==0)
            {
                pipes++;
            }
        }    
    }
    if(pipes==1)
    {
        int index=0;
        char ** temp=(char **)malloc(10*sizeof(char *));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char *)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"|")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp = (char **) realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        for(int i=0;i<index;i++)
        {
            for(int j=0;j<size-1;j++)
            {
                token[j]=token[j+1];
            }
        }
        size=size-index;
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid = fork();
        if(pid==0)
        {
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                if(strcmp(token[size-2],"&")==0)
                {
                    token[size-2]=NULL;
                }
                execvp(token[0],token);
            }
            else if(pid2>0)
            {
                close(fd[1]);
                close(fd[0]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        close(fd[0]);
        close(fd[1]);
        free(temp);
    }
    else if(pipes==2)
    {
        int index=0;
        char ** temp=(char **)malloc(10*sizeof(char *));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char *)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"|")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp = (char **) realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        for(int i=0;i<index;i++)
        {
            for(int j=0;j<size-1;j++)
            {
                token[j]=token[j+1];
            }
        }
        size=size-index;
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid = fork();
        if(pid==0)
        {
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            index=0;
            char ** temp2=(char **)malloc(10*sizeof(char *));
            for(int i=0;i<10;i++)
            {
                temp2[i]=(char *)malloc(100*sizeof(char));
            }
            while(strcmp(token[index],"|")!=0)
            {
                strcpy(temp2[index],token[index]);
                index++;
            }
            index++;
            temp2 = (char **) realloc(temp2,index*sizeof(char*));
            temp2[index-1]=NULL;
            for(int i=0;i<index;i++)
            {
                for(int j=0;j<size-1;j++)
                {
                    token[j]=token[j+1];
                }
            }
            size=size-index;
            int fd2[2];
            pipe(fd2);
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                signal(SIGTSTP,HandleStop);
                parent=getpid();
                pid_t pid3 = fork();
                if(pid3==0)
                {
                    close(fd2[0]);
                    close(fd[1]);
                    dup2(fd[0],0);
                    close(fd[0]);
                    dup2(fd2[1],1);
                    close(fd2[1]);
                    if(strcmp(token[size-2],"&")==0)
                    {
                        token[size-2]=NULL;
                    }
                    execvp(temp2[0],temp2);
                }
                else if(pid3>0)
                {
                    close(fd[1]);
                    close(fd[0]);
                    close(fd2[1]);
                    dup2(fd2[0],0);
                    close(fd2[0]);
                    if(strcmp(token[size-2],"&")==0)
                    {
                        token[size-2]=NULL;
                    }
                    execvp(token[0],token);
                }
            }
            else if(pid2>0)
            {
                close(fd[0]);
                close(fd[1]);
                close(fd2[0]);
                close(fd2[1]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            free(temp2);
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        close(fd[0]);
        close(fd[1]);
        free(temp);
    }
    free(token);
    free(copy);
    return;
}
int checkBothSignsAndPipes(const char * command)
{
    int checkSign=0;
    int checkPipe=0;
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    char * token=strtok(copy," ");
    while(token!=NULL)
    {
        if((strcmp(token,"|")==0))
        {
            checkPipe++;
        }
        else if((strcmp(token,">")==0) || (strcmp(token,"<")==0) || (strcmp(token,">>")==0))
        {
            checkSign++;
        }
        token=strtok(NULL," ");
    }
    free(copy);
    if(checkPipe>0 && checkSign>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void executeBothSignsAndPipes(const char * command)
{
    int pipes=0,less=0,great=0,app=0;
    char * copy = (char *) malloc(100*sizeof(char));
    copy=strcpy(copy,command);
    int size=0;
    char ** token = (char **) malloc (10*sizeof(char*));
    token[0]=strtok(copy," ");
    while(token[size]!=NULL)
    {
        token[++size]=strtok(NULL," ");
    }
    size++;
    token = (char **) realloc(token,size*sizeof(char *));
    removeNewLine(token,size);
    for(int i=0;i<size;i++)
    {
        if(token[i]!=NULL)
        {
            if(strcmp(token[i],"|")==0)
            {
                pipes=i;
            }
            if(strcmp(token[i],">")==0)
            {
                great=i;
            }
            if(strcmp(token[i],"<")==0)
            {
                less=i;
            }
            if(strcmp(token[i],">>")==0)
            {
                app=i;
            }
        }    
    }
    if(less!=0 && great==0 && app==0)
    {
        int index=0;
        char ** temp = (char **) malloc(10*sizeof(char*));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char*)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"<")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp=(char**)realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        char * temp2 = (char *)malloc(100*sizeof(char));
        strcpy(temp2,token[less+1]);
        pipes++;
        for(int i=0;i<pipes;i++)
        {
            for(int j=0;j<size-1;j++)
            {
                token[j]=token[j+1];
            }
        }
        size=size-pipes;
        token = (char **) realloc(token,(size)*sizeof(char *));
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fd1=open(temp2,O_CREAT | O_APPEND | O_RDONLY,0777);
            dup2(fd1,0);
            close(fd1);
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                if(strcmp(token[size-2],"&")==0)
                {
                    token[size-2]=NULL;
                }
                execvp(token[0],token);
            }
            else if(pid2>0)
            {
                close(fd[1]);
                close(fd[0]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        close(fd[1]);
        close(fd[0]);
        free(temp2);
        free(temp);
    }
    else if(less==0 && great!=0 && app==0)
    {
        int index=0;
        char ** temp = (char **) malloc(10*sizeof(char*));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char*)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"|")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp=(char**)realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        char * temp2 = (char *)malloc(100*sizeof(char));
        strcpy(temp2,token[great+1]);
        char ** temp3 = (char**)malloc(10*sizeof(char*));
        int index3=0;
        for(int i=0;i<10;i++)
        {
            temp3[i]=(char*)malloc(100*sizeof(char));
        }
        for(int i=pipes+1;i<great;i++)
        {
            strcpy(temp3[index3],token[i]);
            index3++;
        }
        index3++;
        temp3=(char**)realloc(temp3,(index3)*sizeof(char*));
        temp3[index3-1]=NULL;
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                int fd1=open(temp2,O_CREAT | O_TRUNC | O_RDWR,0777);
                dup2(fd1,1);
                close(fd1);
                if(strcmp(token[size-2],"&")==0)
                {
                    token[size-2]=NULL;
                }
                execvp(temp3[0],temp3);
            }
            else if(pid2>0)
            {
                close(fd[1]);
                close(fd[0]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        close(fd[1]);
        close(fd[0]);
        free(temp2);
        free(temp3);
        free(temp);
    }
    else if(less==0 && great==0 && app!=0)
    {
        int index=0;
        char ** temp = (char **) malloc(10*sizeof(char*));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char*)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"|")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp=(char**)realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        char * temp2 = (char *)malloc(100*sizeof(char));
        strcpy(temp2,token[app+1]);
        char ** temp3 = (char**)malloc(10*sizeof(char*));
        int index3=0;
        for(int i=0;i<10;i++)
        {
            temp3[i]=(char*)malloc(100*sizeof(char));
        }
        for(int i=pipes+1;i<app;i++)
        {
            strcpy(temp3[index3],token[i]);
            index3++;
        }
        index3++;
        temp3=(char**)realloc(temp3,(index3)*sizeof(char*));
        temp3[index3-1]=NULL;
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                int fd1=open(temp2,O_CREAT | O_APPEND | O_RDWR,0777);
                dup2(fd1,1);
                close(fd1);
                if(strcmp(token[size-2],"&")==0)
                {
                    token[size-2]=NULL;
                }
                execvp(temp3[0],temp3);
            }
            else if(pid2>0)
            {
                close(fd[1]);
                close(fd[0]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        close(fd[1]);
        close(fd[0]);
        free(temp2);
        free(temp3);
        free(temp);
    }
    else if(less!=0 && great!=0 && app==0)
    {
        int index=0;
        char ** temp = (char **) malloc(10*sizeof(char*));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char*)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"<")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp=(char**)realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        char * temp2 = (char *)malloc(100*sizeof(char));
        strcpy(temp2,token[less+1]);
        char * temp4 = (char *)malloc(100*sizeof(char));
        strcpy(temp4,token[great+1]);
        char ** temp3 = (char**)malloc(10*sizeof(char*));
        int index3=0;
        for(int i=0;i<10;i++)
        {
            temp3[i]=(char*)malloc(100*sizeof(char));
        }
        for(int i=pipes+1;i<great;i++)
        {
            strcpy(temp3[index3],token[i]);
            index3++;
        }
        index3++;
        temp3=(char**)realloc(temp3,(index3)*sizeof(char*));
        temp3[index3-1]=NULL;
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fd1=open(temp2,O_CREAT | O_APPEND | O_RDONLY,0777);
            dup2(fd1,0);
            close(fd1);
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                int fd2=open(temp4,O_CREAT | O_TRUNC | O_RDWR,0777);
                dup2(fd2,1);
                close(fd2);
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                if(strcmp(token[size-2],"&")==0)
                {
                    token[size-2]=NULL;
                }
                execvp(temp3[0],temp3);
            }
            else if(pid2>0)
            {
                close(fd[1]);
                close(fd[0]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        close(fd[1]);
        close(fd[0]);
        free(temp2);
        free(temp3);
        free(temp4);
        free(temp);
    }
    else if(less!=0 && great==0 && app!=0)
    {
        int index=0;
        char ** temp = (char **) malloc(10*sizeof(char*));
        for(int i=0;i<10;i++)
        {
            temp[i]=(char*)malloc(100*sizeof(char));
        }
        while(strcmp(token[index],"<")!=0)
        {
            strcpy(temp[index],token[index]);
            index++;
        }
        index++;
        temp=(char**)realloc(temp,index*sizeof(char*));
        temp[index-1]=NULL;
        char * temp2 = (char *)malloc(100*sizeof(char));
        strcpy(temp2,token[less+1]);
        char * temp4 = (char *)malloc(100*sizeof(char));
        strcpy(temp4,token[app+1]);
        char ** temp3 = (char**)malloc(10*sizeof(char*));
        int index3=0;
        for(int i=0;i<10;i++)
        {
            temp3[i]=(char*)malloc(100*sizeof(char));
        }
        for(int i=pipes+1;i<app;i++)
        {
            strcpy(temp3[index3],token[i]);
            index3++;
        }
        index3++;
        temp3=(char**)realloc(temp3,(index3)*sizeof(char*));
        temp3[index3-1]=NULL;
        int fd[2];
        pipe(fd);
        signal(SIGTSTP,HandleStop);
        parent=getpid();
        pid_t pid=fork();
        if(pid==0)
        {
            int fd1=open(temp2,O_CREAT | O_APPEND | O_RDONLY,0777);
            dup2(fd1,0);
            close(fd1);
            close(fd[0]);
            dup2(fd[1],1);
            close(fd[1]);
            if(strcmp(token[size-2],"&")==0)
            {
                token[size-2]=NULL;
            }
            execvp(temp[0],temp);
        }
        else if(pid>0)
        {
            signal(SIGTSTP,HandleStop);
            parent=getpid();
            pid_t pid2=fork();
            if(pid2==0)
            {
                int fd2=open(temp4,O_CREAT | O_APPEND | O_RDWR,0777);
                dup2(fd2,1);
                close(fd2);
                close(fd[1]);
                dup2(fd[0],0);
                close(fd[0]);
                if(strcmp(token[size-2],"&")==0)
                {
                    token[size-2]=NULL;
                }
                execvp(temp3[0],temp3);
            }
            else if(pid2>0)
            {
                close(fd[1]);
                close(fd[0]);
                if(strcmp(token[size-2],"&")!=0)
                {   
                    child=pid2;
                    waitpid(pid2,NULL,WUNTRACED);
                }
                else
                {
                    Enqueue(JobStack,pid2);
                }
            }
            else
            {
                printf("\nFork Failed!\n");
                exit(-1);
            }
            if(strcmp(token[size-2],"&")!=0)
            {   
                child=pid;
                waitpid(pid,NULL,WUNTRACED);
            }
            else
            {
                Enqueue(JobStack,pid);
            }
        }
        else
        {
            printf("Fork Failed!");
            exit(-1);
        }
        close(fd[1]);
        close(fd[0]);
        free(temp2);
        free(temp3);
        free(temp4);
        free(temp);
    }
    free(token);
    free(copy);
    return;
}
void removeNewLine(char ** token,int size)
{
    for(int i=0;i<size;i++)
    {
        if(token[i]!=NULL)
        {
            for(int j=0;token[i][j]!='\0';j++)
            {
                if(token[i][j]=='\n')
                {
                    for(int k=j;token[i][k]!='\0';k++)
                    {
                        token[i][k]=token[i][k+1];
                    }
                }
            }
        }
    }
    return;
}
void removeQuotes(char * token)
{
    for(int i=0;token[i]!='\0';i++)
    {
        if(token[i]=='\"')
        {
            for(int j=i;token[j]!='\0';j++)
            {
                token[j]=token[j+1];
            }
        }
    }
    return;
}