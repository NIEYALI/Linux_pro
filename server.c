/*************************************************************************
	> File Name: server.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Sat Sep 11 07:10:56 2021
 ************************************************************************/
 //run: gcc server.c -lpthread -o server
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/epoll.h>

#define SERV_PORT 8000
#define MAXLINE 8000

#define prrexit(msg){\
    perror(msg);\
    exit(1);\
}

typedef struct Task{
    int fd;
    struct Task *next;
}Task;

typedef struct Task_pool{
    Task *head;
    Task *tail;
    pthread_mutex_t lock;
    pthread_cond_t havetask;
}Task_pool;

Task_pool *task_pool_init(){
    Task_pool *tp=(Task_pool *)malloc(sizeof(Task_pool));
    tp->head=NULL;
    tp->tail=NULL;
    pthread_mutex_init(&tp->lock,NULL);
    pthread_cond_init(&tp->havetask,NULL);

    return tp;
}

void Task_pool_push(Task_pool *tp,int fd){
    pthread_mutex_lock(&tp->lock);

    Task *t=(Task *)malloc(sizeof(Task));
    t->fd=fd;
    t->next=NULL;

    if(!tp->tail){
        tp->head=tp->tail=t;
    }else{
        tp->tail->next=t;
        tp->tail=t;
    }

    pthread_cond_broadcast(&tp->havetask);
    pthread_mutex_unlock(&tp->lock);
}

Task task_pool_pop(Task_pool *tp){
    pthread_mutex_lock(&tp->lock);

    while(tp->head==NULL)
        pthread_cond_wait(&tp->havetask,&tp->lock);


    Task tmp, *k;
    k=tp->head;
    tmp=*k;
    tp->head=tp->head->next;

    if(!tp->head)
        tp->tail=NULL;

    free(k);
    pthread_mutex_unlock(&tp->lock);

    return tmp;
}

void Task_pool_free(Task_pool *tp){
    pthread_mutex_lock(&tp->lock);
    Task *p=tp->head, *k;

    while(p){
        k=p;
        p=p->next;
        free(k);
    }
    tp->head=NULL;

    pthread_mutex_unlock(&tp->lock);
    pthread_mutex_destroy(&tp->lock);
    pthread_cond_destroy(&tp->havetask);
    free(tp);
    return ;
}


void *up_server(void *arg){
    
    pthread_detach(pthread_self());

    char buf[MAXLINE];
    int n, i;

    Task_pool *tp=arg;


    while(1){
        Task tmp=task_pool_pop(tp);
        int connfd=tmp.fd;
        printf("get task id=%d\n",connfd);
        if(i){
            n=read(connfd,buf,MAXLINE);
            write(1,buf,n);

            //for(i=0;i<n;i++)
            //   buf[i]=toupper[buf[i]];

            char rspbuf[100]={"HTTP/1.1 404 hahaerr\r\nContent-Type:text/html\r\n\r\n<html><body><H3>your page losses</H3></body></html>"};
            write(connfd,rspbuf,strlen(rspbuf));
        }
        printf("finish task fd=%d\n",connfd);
        close(connfd);
    }
    return (void *)0;
}



int main(void){

    struct sockaddr_in serveraddr, cliaddr;
    int listenfd, connfd;
    socklen_t cliaddr_len;

    char str[INET_ADDRSTRLEN];
    int n,i;

    Task_pool *tp=task_pool_init();

    //多线程
    pthread_t tid;
    for(i=0;i<4;i++){
        pthread_create(&tid,NULL,up_server,(void *)tp);
        printf("new thread is %#lx\n",tid);
    }


    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0)
        prrexit("socket");

    int epfd=epoll_create(256);
    struct epoll_event ev,events[256];
    ev.events=EPOLLIN | EPOLLET;
    ev.data.fd=listenfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

    //服务器ip地址：端口初始化
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(SERV_PORT);
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))<0)
        prrexit("blind");


    if(listen(listenfd,2)<0)
        prrexit("listend");

    printf("Accepting connection...\n");
    while(1){
        
        int nfds=epoll_wait(epfd,events,256,-1);
        for(i=0;i<nfds;i++){
            if(events[i].data.fd==listenfd){
                cliaddr_len=sizeof(cliaddr);
                connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
                if(connfd<0)
                    prrexit("accept");

                printf("received from %s:%d\n",\
                      inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),
                      ntohs(cliaddr.sin_port));

                ev.events=EPOLLIN | EPOLLET;
                ev.data.fd=connfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);

            }else if(events[i].events &EPOLLIN){
                int clifd=events[i].data.fd;
                if(clifd<3)
                    continue;

                Task_pool_push(tp,clifd);
            }
        }
    }
   
   Task_pool_free(tp);
   return 0;
}
