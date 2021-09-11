/*************************************************************************
	> File Name: hello.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Thu Aug 19 13:13:09 2021
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#define LEN 14

char msg[LEN] = "hello, world!\n";
int main(void){

    write(1,msg,LEN);
    _exit(77);
}
