/*************************************************************************
	> File Name: yalshell.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Mon Aug 30 11:24:27 2021
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<sys/fcntl.h>
#include<sys/stat.h>

#define LEN 1024

char *trim(char *str){
    int head=0;
    int tail=strlen(str)-1;

    while(isspace(str[head]))
        head++;
    while(isspace(str[tail]))
        str[tail--]=0;
    return str+head;
}
int main(void){
    char buff[LEN];
    while(1){
        printf("input$");
        //scanf("%s",buff);

        fgets(buff,LEN,stdin);
        buff[strlen(buff)-1]=0;
        printf("cmd:[%s]\n",buff);

        if(!strcmp(buff,"exit")){
            printf("exit^_^\n");
            break;
        }
        //Create child processing
        pid_t pid=fork();
        if(pid<0){
            perror("fork");
            exit(1);
        }
        //父进程等待子进程，继续下一轮
        if(pid){
            wait(NULL);
            continue;
        }
        //Child processing
        //execl(cmd);
        //perror();
        //exit(1);
        int redfd=-1;
        if(strstr(buff,"<")){
            redfd=0;
        }
        if(strstr(buff,">")){
            redfd=1;
        }

        char *cmd=NULL;
        if(redfd!=-1){
            char *token=strtok(buff,"<>");
            //printf("token:[%s]\n",token);
            cmd=token;
            token=strtok(NULL,"<>");
            //printf("token::[%s]\n,token");
            token=trim(token); 
            int fd;
            if(redfd)
                fd=open(token,O_RDWR | O_CREAT,0644);
            else
                fd=open(token,O_RDWR);
            
            if(fd<0){
                perror(token);
                exit(1);
                }
                //token=trim(token);
                printf("open %s successfully ~\n",token);
                dup2(fd,redfd);
    
        } else{
            cmd=buff;
        }

        int i=0;
        char *argarr[20];
        char *tk=strtok(cmd," ");
        while(tk){
            argarr[i++]=tk;
            tk=strtok(NULL," ");
        }
        
        argarr[i]=tk;
        execvp(argarr[0],argarr);
        perror(argarr[0]);
        exit(1);
        }
    return 0;
}
