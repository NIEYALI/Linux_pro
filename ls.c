/*************************************************************************
	> File Name: ls.c
	> Author: Yali Nie
	> Mail: yali.nie@miun.se 
	> Created Time: Thu Aug 26 07:11:08 2021
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<pwd.h>
#include<stdlib.h>
#include<grp.h>
#include<time.h>
void ls_al(char *path){
    if(chdir(path)<0){
        perror("path");
        exit(1);
    }
    DIR *dir=opendir(".");
    struct dirent *dir_info;
    struct stat info_buf;
    char file_type='-';
    char power[10]=("---------");
    while(dir_info=readdir(dir)){
        printf("%s\n",dir_info->d_name);
        if(stat(dir_info->d_name,&info_buf)<0){
          perror("stat");
          exit(1);
        }
        switch(info_buf.st_mode & S_IFMT){
            case S_IFSOCK:
                file_type='s';
                break;
            case S_IFLNK:
                file_type='l';
                break;   
            case S_IFREG:
                file_type='-';
                break;
            case S_IFBLK:
                file_type='b';
                break;
            case S_IFDIR:
                file_type='d';
                break;
            case S_IFCHR:
                file_type='c';
                break;
            case S_IFIFO:
                file_type='p';
                break;
            default:
                break;
        }

        power[0]=info_buf.st_mode & S_IRUSR?'r':'-';
        power[1]=info_buf.st_mode & S_IWUSR?'w':'-';
        power[2]=info_buf.st_mode & S_IXUSR?'x':'-';

        power[3]=info_buf.st_mode & S_IRGRP?'r':'-';
        power[4]=info_buf.st_mode & S_IWGRP?'w':'-';
        power[5]=info_buf.st_mode & S_IXGRP?'x':'-';

        power[6]=info_buf.st_mode & S_IROTH?'r':'-';
        power[7]=info_buf.st_mode & S_IWOTH?'w':'-';
        power[8]=info_buf.st_mode & S_IXOTH?'x':'-';

        struct passwd *pwd= getpwuid(info_buf.st_uid);
        struct group *grp=getgrgid(info_buf.st_gid);
        struct tm *mtime=localtime(&info_buf.st_mtime);

        printf("%c%s %3ld %s %s %8ld %4d/%02d/%02d %02d:%02d:%02d %s\n",
               file_type,
               power,
               info_buf.st_nlink,
               pwd->pw_name,
               grp->gr_name,
               info_buf.st_size,
               mtime->tm_year+1900,
               mtime->tm_mon+1,
               mtime->tm_mday,
               mtime->tm_hour,
               mtime->tm_min,
               mtime->tm_sec,

               dir_info->d_name);
    }
    
    return ;
}
int main(int argc,char **argv){
    if(argc!=2){
        printf("usage:cmd + pathname\n");
        return 1;
    }
    ls_al(argv[1]);
    return 0;
}
