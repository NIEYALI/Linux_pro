/*************************************************************************
	> File Name: test3.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Mon Aug 30 10:05:09 2021
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, char **argv){
    if(argc!=2){
        printf("usage:./a.out + n\n");
        exit(1);
    }
    int n=atoi(argv[1]);
    while(n--){
        pid_t pid=fork();
        if(pid<0){
            perror("fork");
            exit(1);
        }
        if(pid){
            printf("==================\n");
            printf("self=%d\n",getpid());
            printf("father=%d\n",getppid());
            printf("child=%d\n",pid);
            printf("==================\n");

            wait(NULL);
            printf("%d over\n",getpid());

            break;
        }
    }
    
    return 0;
}
