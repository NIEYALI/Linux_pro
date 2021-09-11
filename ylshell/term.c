/*************************************************************************
	> File Name: term.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Sun Sep  5 19:05:22 2021
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main(void){
    printf("%s\n",ttyname(0));
    printf("%s\n",ttyname(1));
    printf("%s\n",ttyname(2));
   // printf("%s\n",ttyname(3));
    return 0;
}
