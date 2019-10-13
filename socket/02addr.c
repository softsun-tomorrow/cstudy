#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
    unsigned long addr = inet_addr("192.168.0.100"); //addr 是网络字节序
    printf("addr=%u\n", ntohl(addr)); //网络字节序转换为主机字节序

    struct in_addr ipaddr;
    ipaddr.s_addr = addr;
    printf("%s\n", inet_ntoa(ipaddr)); //将地址结构转换为点逢十进制的IP形式
    //inet_aton 将点逢十进制的IP地址转换为网络字节序的地址  地址输出结构
    //inet_addr 同上，只不过输出的是直接转换为32位整数
    return 0;
}
