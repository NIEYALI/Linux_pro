/*************************************************************************
	> File Name: print_a.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Thu Sep  9 11:07:40 2021
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

int cntarr[3];

//pthread_mutex_t plock=PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t lastp[3];

sem_t sarr[3];

//pthread_mutex_init(0);
void *showChar(void *arg){
    int which=(int)(long)arg;

   // int which=int arg;
    while(cntarr[which]<10){
       // pthread_mutex_lock(&plock);
        //pthread_cond_wait(lastp+(which+2)%3,&plock);
        sem_wait(sarr+which);
        printf("%c","ABC"[which]);
       // printf("%c",'A'+[which]);
        cntarr[which]++;
        sem_post(sarr+(which+1)%3);
       // pthread_cond_signal(lastp+which);
        //pthread_cond_broadcast();
        //pthread_mutex_unlock(&plock);
    }
    return (void *)0;
}
int main(void){
    int i;
    for(i=0;i<3;i++)
        sem_init(sarr+i,0,0);
    //for(i=0;i<3;i++)
      //  pthread_cond_init(lastp+i,NULL);
    pthread_t tid[3];
    //pthread_create(tid,NULL,showChar,(void *)1);
    //pthread_create(tid+1,NULL,showChar,(void *)2);
    //pthread_create(tid+2,NULL,showChar,(void *)3);
   // int i;
    for(i=0;i<3;i++)    
         pthread_create(tid+i,NULL,showChar,(void *)(long)i);
    
    sem_post(sarr);
    for(i=1;i<3;i++)
        sem_destroy(sarr+i);
    //sleep(1);
   // pthread_cond_signal(lastp+2);
    for(i=0;i<3;i++)  
        pthread_join(tid[i], NULL);
    
   // pthread_mutex_destroy(&plock);
    //for(i=0;i<3;i++)
       // pthread_cond_destroy(lastp+i);

    return 0;
}
