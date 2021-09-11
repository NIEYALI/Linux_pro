/*************************************************************************
	> File Name: proc3.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Mon Aug 30 09:31:47 2021
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void){

    pid_t pid=fork();
    if(pid<0){
        perror("fork");
        exit(1);
    }
    if(!pid)
        execlp("./p1.out","x",NULL);
    pid=fork(); 
    if(pid<0){
        perror("fork");
        exit(1);
    }
    if(!pid)
        execlp("./p2.out","x",NULL);

    int sts;
    int ret;
    while(1){
        wait(&sts);
        if(WIFEXITED(sts)){
            ret=WEXITSTATUS(sts);
            pid=fork();
            if(!pid){
                if(ret==1)
                    execlp("./p1.out","x",NULL);
                else
                    execlp("./p2.out","x",NULL);
            }
        }
    }
    return 0;
}
