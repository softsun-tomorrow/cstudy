#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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

//添加readline，主要解决粘包问题

ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char *bufp = (char*)buf;

    while(nleft > 0)
    {
        if((nread = read(fd, bufp, nleft)) < 0)
        {
            if(errno == EINTR)
                continue;  //信号中断，则继续
            return -1;
        }
        else if(nread ==0)
            return count - nleft; //全部要读取的字节数 - 剩余的字节数;
        bufp += nread;
        nleft -= nread;
    }
    return count;
}

ssize_t writen(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwritten;
    char *bufp = (char*)buf;

    while(nleft > 0)
    {
        if((nwritten = write(fd, bufp, nleft)) < 0)
        {
            if(errno == EINTR)
                continue;  //信号中断，则继续
            return -1;
        }
        else if(nwritten ==0)
            continue;
        bufp += nwritten;
        nleft -= nwritten;
    }
    return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while(1)
    {
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if(ret==-1 && errno==EINTR)
            continue;
        return ret;
    }
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;
    char *bufp = buf;
    int nleft = maxline;
    while(1)
    {
        ret = recv_peek(sockfd, bufp, nleft);
        if(ret<0)
            return ret;
        else if(ret==0)
            return ret;
        nread = ret;
        int i;
        for(i=0; i<nread; i++)
        {
            if(bufp[i]=='\n')
            {
                ret = readn(sockfd, bufp, i+1);
                if(ret != (i+1))
                    exit(EXIT_FAILURE);
                return ret;
            }
        }
        
        if(nread > nleft)
            exit(EXIT_FAILURE);

        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if(ret!=nread)
            exit(EXIT_FAILURE);
        bufp += nread;
    }
    return -1;
}

void echo_srv(int conn)
{
    char recvbuf[1024];
    while(1)
    {
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = readline(conn, recvbuf, 1024);
        //返回值为0 客户端进程关闭了
        if(ret==-1) //失败了
        {
            ERR_EXIT("readline");
        }
        else if(ret==0)
        {
            printf("client_close\n");
            break;
        }
        fputs(recvbuf, stdout);
        writen(conn, recvbuf, strlen(recvbuf));
    }

}

void handle_sigchld(int sig)
{
//    wait(NULL);
    while(waitpid(-1, NULL, WNOHANG) > 0); //循环保证信号能够所有子进程返回执行，而不会被忽略
}

//多个客户端和一个服务器通信
int main(void)
{
    //避免僵尸进程
    //方法一
    //signal(SIGCHLD, SIG_IGN);
    //方法二
    signal(SIGCHLD, handle_sigchld);
    int listenfd;
    if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
//    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
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
    socklen_t peerlen;
    int conn;
/*
    pid_t pid;
    while(1)
    {
        if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
            ERR_EXIT("accept");

        printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
        
        pid = fork();
        if(pid == -1)
            ERR_EXIT("fork");
        if(pid==0)
        {
            close(listenfd);
            echo_srv(conn);
            exit(EXIT_SUCCESS); //一旦函数返回,进程就退出
        }
        else
        {
            close(conn);
        }
    }
*/
    //使用使用select实现多并发
    int i;
    int client[FD_SETSIZE];
    for(i=0;i<FD_SETSIZE; i++)
        client[i] = -1; //-1表示空闲的
    int nready; //检测到的事件个数
    int maxfd = listenfd; //监听的套接口
    fd_set rset;
    fd_set allset;
    FD_ZERO(&rset);
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset); //将监听的套接口放到集合中
    while(1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(nready==1)
        {
            if(errno==EINTR) continue;
            ERR_EXIT("select");
        }
        if(nready==0) continue;
        if(FD_ISSET(listenfd, &rset))
        {
            peerlen = sizeof(peeraddr);
            conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen);
            if(conn==-1)
                ERR_EXIT("accept");
            for(i=0; i<FD_SETSIZE; i++)
            {
                if(client[i]<0)
                {
                    client[i] = conn;
                    break;
                }
            }
            if(i==FD_SETSIZE)
            {
                fprintf(stderr, "too many clients\n");
                exit(EXIT_FAILURE);
            }
            printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

            FD_SET(conn, &allset);
            if(conn>maxfd)
                maxfd = conn;
            if(--nready <=0)
                continue;
        }
        for(i=0; i<FD_SETSIZE; i++)
        {
            conn = client[i];
            if(conn==-1)
                continue;
            if(FD_ISSET(conn, &rset))
            {
                char recvbuf[1024] = {0};
                int ret = readline(conn, recvbuf, 1024);
                if(ret==-1)
                    ERR_EXIT("readline");
                if(ret==0)
                {
                    printf("client close\n");
                    FD_CLR(conn, &allset);
                }
                fputs(recvbuf, stdout);
                writen(conn, recvbuf, strlen(recvbuf));
                if(--nready<=0)
                    break;
            }
        }
    }

    return 0;
}
