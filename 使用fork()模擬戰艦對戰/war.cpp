#include<iostream>
#include<ctime>
#include<cstdlib>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
#include<errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#define MAXSIZE 1024*1024*16
#define FILENAME "shm.test"

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

using namespace std;
struct loc{
    int x;
    int y;
};

int main(int argc,char *argv[]){
    int p1=std::stoi(argv[1],nullptr,0);
    int p2=std::stoi(argv[2],nullptr,0);
    int p3=std::stoi(argv[3],nullptr,0);
    loc parent[2],child[2];
    pid_t pid = fork();

    /*int fd = shm_open(FILENAME, O_CREAT | O_TRUNC | O_RDWR, 0777);
    if (fd == -1) {
        perror("open failed:");
        exit(1);
    }

    if (ftruncate(fd, MAXSIZE) == -1) {
        perror("ftruncate failed:");
        exit(1);
    }

    struct stat buf;
    if (fstat(fd, &buf) == -1) {
        perror("fstat failed:");
        exit(1);
    }
    printf("the shm object size is %ld\n", buf.st_size);

    sleep(30);

    if (shm_unlink(FILENAME) == -1) {
        perror("shm_unlink failed:");
        exit(1);
    }
    printf("shm_unlink %s success\n", FILENAME);*/
   
  int r;

  const char *memname = "sample";
  const size_t region_size = sysconf(_SC_PAGE_SIZE);

  int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
  if (fd == -1)
    error_and_die("shm_open");

  r = ftruncate(fd, region_size);
  if (r != 0)
    error_and_die("ftruncate");

  void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    error_and_die("mmap");
  close(fd);

/*
  r = munmap(ptr, region_size);
  if (r != 0)
    error_and_die("munmap");

  r = shm_unlink(memname);
  if (r != 0)
    error_and_die("shm_unlink");
*/
    if(pid>0){
    cout<<"["<<getpid()<<" Parent]: "<<"Random Seed "<<p1<<endl;
    raise(SIGCONT);
    kill(getpid()+1,SIGCONT);
    int wstatus;
    waitpid(pid,&wstatus,WUNTRACED);
    srand(p1);
    int b1,c1,temp=rand()%(1-0+1)+0;
    if(temp==0){
        b1=rand()%(2-0+1)+0;
        c1=rand()%(3-0+1)+0;
        parent[0].x=b1;
        parent[0].y=c1;
        parent[1].x=b1+1;
        parent[1].y=c1;
        cout<<"["<<getpid()<<" Parent]: "<<"The gunboat: ("<<b1<<","<<c1<<")";
        cout<<"("<<b1+1<<","<<c1<<")\n";
    }
    else{
        b1=rand()%(3-0+1)+0;
        c1=rand()%(2-0+1)+0;
        parent[0].x=b1;
        parent[0].y=c1;
        parent[1].x=b1;
        parent[1].y=c1+1;
        cout<<"["<<getpid()<<" Parent]: "<<"The gunboat: ("<<b1<<","<<c1<<")";
        cout<<"("<<b1<<","<<c1+1<<")\n";
    }
    int status;
    raise(SIGCONT);
    kill(getpid()+1,SIGCONT);
    waitpid(pid,&status,WUNTRACED);
    int cpid=*(u_long *) ptr;
    u_long *d = (u_long *) ptr;
    *d=getpid();
    raise(SIGCONT);
    kill(getpid()+1,SIGCONT);
    waitpid(pid,&status,WUNTRACED);
    int x=0,ph1=2;
    while(ph1>0){
        srand(x+99);
        int bb1=rand()%(3-0+1)+0;
        int bb2=rand()%(3-0+1)+0;
        cout<<"["<<getpid()<<" Parent]: "<<"The bombing: ("<<bb1<<","<<bb2<<")\n";
        u_long *d = (u_long *) ptr;
        *d=bb1*10+bb2;
        raise(SIGCONT);
        kill(getpid()+1,SIGCONT);
        waitpid(pid,&status,WUNTRACED);
        int g=*(u_long *) ptr;
        if(g==-1)
        exit(0);
        int bomby=g%10;
        int bombx=g/10;
        x++;
        if((parent[0].x==bombx&&parent[0].y==bomby)||(parent[1].x==bombx&&parent[1].y==bomby)){
          ph1--;
          if(parent[0].x==bombx&&parent[0].y==bomby){
            parent[0].x=-1;
            parent[0].y=-1;
          }
          else{
            parent[1].x=-1;
            parent[1].y=-1;
          }
          if(ph1==0){
            cout<<"["<<getpid()<<" Parent]: "<<"hit and sinking\n";
            cout<<"["<<getpid()<<" Parent]: "<<cpid<<" wins with "<<x<<" bombs\n";
            u_long *a = (u_long *) ptr;
            *a=-1;
            exit(0);
          }
          else {
            cout<<"["<<getpid()<<" Parent]: "<<"hit\n";
          } 
        }
        else {
          cout<<"["<<getpid()<<" Parent]: "<<"missed\n";
        }
    }
    }
    else if(pid==0){
    cout<<"["<<getpid()<<" Child]: "<<"Random Seed "<<p2<<endl;
    raise(SIGSTOP);
    srand(p2);
    int b1,c1,temp=rand()%(1-0+1)+0;
    if(temp==0){
        b1=rand()%(2-0+1)+0;
        c1=rand()%(3-0+1)+0;
        child[0].x=b1;
        child[0].y=c1;
        child[1].x=b1+1;
        child[1].y=c1;
        cout<<"["<<getpid()<<" Child]: "<<"The gunboat: ("<<b1<<","<<c1<<")";
        cout<<"("<<b1+1<<","<<c1<<")\n";
    }
    else{
        b1=rand()%(3-0+1)+0;
        c1=rand()%(2-0+1)+0;
        child[0].x=b1;
        child[0].y=c1;
        child[1].x=b1;
        child[1].y=c1+1;
        cout<<"["<<getpid()<<" Child]: "<<"The gunboat: ("<<b1<<","<<c1<<")";
        cout<<"("<<b1<<","<<c1+1<<")\n";
    }
    u_long *a = (u_long *) ptr;
    *a=getpid();
    raise(SIGSTOP);
    int ppid=*(u_long *) ptr;
    raise(SIGSTOP);
    int y=0,ph2=2;
    while(ph2>0){
        int gg=*(u_long *) ptr;
        if(gg==-1)
        exit(0);
        int bomby=gg%10;
        int bombx=gg/10;
        y++;
        if((child[0].x==bombx&&child[0].y==bomby)||(child[1].x==bombx&&child[1].y==bomby)){
          ph2--;
          if(child[0].x==bombx&&child[0].y==bomby){
            child[0].x=-1;
            child[0].y=-1;
          }
          else{
            child[1].x=-1;
            child[1].y=-1;
          }
          if(ph2==0){
            cout<<"["<<getpid()<<" Child]: "<<"hit and sinking\n";
            cout<<"["<<getpid()<<" Child]: "<<ppid<<" wins with "<<y<<" bombs\n";
            u_long *aa = (u_long *) ptr;
            *aa=-1;
            exit(0);
          }
          else {
            cout<<"["<<getpid()<<" Child]: "<<"hit\n";
          } 
        }
        else {
          cout<<"["<<getpid()<<" Child]: "<<"missed\n";
        }
        srand(y+ph2);
        int bb1=rand()%(3-0+1)+0;
        int bb2=rand()%(3-0+1)+0;
        cout<<"["<<getpid()<<" Child]: "<<"The bombing: ("<<bb1<<","<<bb2<<")\n";
        u_long *d = (u_long *) ptr;
        *d=bb1*10+bb2;
        raise(SIGSTOP);
    }
    }
}



