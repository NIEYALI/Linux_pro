/*************************************************************************
	> File Name: process.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Mon Aug 30 08:50:05 2021
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main(int argc, char **argv){
    int i;
    for(i=0;i<3;i++){
        if(fork()==0){
            printf("the child %d\n",getpid());
        }
        else 
            printf("the parent %d\n",getpid());
    }
    //sleep();
}
