#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


#define ERR_EXIT(m) \
        do \
        { \
            perror(m); \
        } while(0)

struct packet
{
    int len;
    char buf[1024];
};

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

int main(void)
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");

    //使用自定义协议包
    struct packet sendbuf;
    struct packet recvbuf;
    memset(&sendbuf, 0, sizeof(sendbuf));
    memset(&recvbuf, 0, sizeof(recvbuf));
    int n;
    while(fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin)!=NULL)
    {
        n = strlen(sendbuf.buf);
        sendbuf.len = htonl(n);
        writen(sock, &sendbuf, 4+n);//发送头部4个字节+数据包的长度

        int ret = readn(sock, &recvbuf.len, 4);
        //返回值为0 客户端进程关闭了
        if(ret==-1) //失败了
        {
            ERR_EXIT("read");
        }   
        else if(ret<4)
        {
            printf("client_close\n");
            break;
        }
        n = ntohl(recvbuf.len);
        ret = readn(sock, recvbuf.buf, n);
        if(ret==-1) //失败了
        {   
            ERR_EXIT("read");
        }
        else if(ret<n)
        {
            printf("client_close\n");
            break;
        }
        
        fputs(recvbuf.buf, stdout);
        memset(&sendbuf, 0, sizeof(sendbuf));
        memset(&recvbuf, 0, sizeof(recvbuf));
    }
    close(sock);

    return 0;
}
