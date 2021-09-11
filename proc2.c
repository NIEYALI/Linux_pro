/*************************************************************************
	> File Name: proc1.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Mon Aug 30 09:22:40 2021
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
int main(void){
    printf("I am process 2\n");

    sleep(3);
    return 2;
}
