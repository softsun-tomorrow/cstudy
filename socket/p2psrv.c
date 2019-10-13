#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
            perror(m); \
        } while(0)
//信号处理函数
void handler(int sig)
{
    printf("recv a sig=%d\n", sig);
    exit(EXIT_SUCCESS);
}

//点对点的通信 创建两个进程一个用户接收数据，一个用来发送数据
int main(void)
{
    int listenfd;
    if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
//    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
//    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    inet_aton("127.0.0.1", &servaddr.sin_addr);
    
    int on=1;
    //开启地址重复利用
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)    
        ERR_EXIT("setsockopt");

    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");
    if(listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    int conn;
    if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
        ERR_EXIT("accept");

    printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

    pid_t pid;
    pid = fork();
    if(pid==1)
        ERR_EXIT("fork");
    if(pid==0)
    {
        signal(SIGUSR1, handler);
        char sendbuf[1024] = {0};
        while(fgets(sendbuf, sizeof(sendbuf), stdin)!=NULL)
        {            
            write(conn, sendbuf, strlen(sendbuf));
            memset(sendbuf, 0, sizeof(sendbuf));//清空
        }
        printf("child close");
        exit(EXIT_SUCCESS);
    }
    else //父进程用户接收数据
    {
        char recvbuf[1024];
        while(1)
        {
            memset(recvbuf, 0, sizeof(recvbuf));
            int ret = read(conn, recvbuf, sizeof(recvbuf));
            if(ret==-1)
                ERR_EXIT("read");
            if(ret==0)
            {
                printf("peer close\n");
                break;//跳出循环，程序结束
            }
            fputs(recvbuf, stdout);
        }
        printf("parent close\n");
        //父进程退出后，向子进程发送一个信号
        kill(pid, SIGUSR1);
        exit(EXIT_SUCCESS);
    }
    close(conn);
    close(listenfd);

    return 0;
}
