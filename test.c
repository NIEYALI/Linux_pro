/*************************************************************************
	> File Name: test.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Thu Aug 19 16:02:00 2021
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 2048
typedef struct Node{
    char *wd;
    char *trans;
    struct Node *next;
}Node;


Node * loadDict(Node *head){
    char buff[LEN];
    FILE *fp = fopen("./dict.txt","r");
    if (!fp){
        perror("open dict.txt file");
        exit(1);
    }
    while(fgets(buff,LEN,fp)){
        Node *nn = (Node *)malloc(sizeof(Node));
        nn->wd=(char *)malloc(sizeof(char)*(strlen(buff)+1));
        strcpy(nn->wd,buff+1);
        nn->wd[strlen(nn->wd)-1]='\0';
        
        fgets(buff,LEN,fp);
        nn->trans=(char *)malloc(sizeof(char)*(strlen(buff)+1));
        strcpy(nn->trans,buff+6);
        //nn->trans[strlen(nn->trans)-1]='\0';

        nn->next=head;
        head=nn;
    }
    fclose(fp);
    return head;
}

Node *findTrans(Node *head,char *iptwd){
    Node *p=head;
    while(p && strcmp(iptwd,p->wd))
        p=p->next;

    return p;
    
}

void freeSys(Node *head){
    Node*k,*p=head;

    while(p){
        k=p;
        p=p->next;
        free(k->wd);
        free(k->trans);
        free(k);
    }
}
int main(void){
    Node *head=NULL;
    head=loadDict(head); 
    char iptwd[100];
    while(1){
        printf("please input a word:");
        scanf("%s",iptwd);
        if(!strcmp(iptwd,"_quit"))
            break;
        Node *tmp=findTrans(head,iptwd);

        if(tmp)
            printf("%s",tmp->trans);
        else
            printf("your word not found!^_^\n");
    }
    printf("see you ~~~~\n");
    freeSys(head);
    return 0;
}
