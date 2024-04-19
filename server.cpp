#include <stdio.h>
#include <winsock2.h>
#include<conio.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

//指向服务器IP
#define ServerIp "192.168.0.103"
#define LocalIP

#ifdef LocalIP
#define ServerIp "127.0.0.1"
#endif


int main(){
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);
 
 
    //创建套接字
    SOCKET serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
 
    //绑定套接字
    sockaddr_in serverSockAddr;   
    memset(&serverSockAddr, 0, sizeof(serverSockAddr));  //每个字节都用0填充
    serverSockAddr.sin_family = PF_INET;  //使用IPv4地址
    serverSockAddr.sin_addr.s_addr = inet_addr(ServerIp);  //具体的IP地址
    serverSockAddr.sin_port = htons(1234);  //端口
    bind(serverSock, (SOCKADDR*)&serverSockAddr, sizeof(SOCKADDR));
 
 
    //进入监听状态
    listen(serverSock, 20);
    SOCKET clientSock;
    //接收客户端请求
    sockaddr clntAddr;
    int nSize = sizeof(sockaddr);
        //向客户端发送数据
    clientSock = accept(serverSock, (sockaddr*)&clntAddr, &nSize);
    const char quick = ' ';
    //设置套接字选项
    int rc = setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, &quick, sizeof(char));
    printf("client join");
    while(true){
        char ch = ' ';
        if(_kbhit()){
            ch = _getch();
            printf("%s\n",&ch);
            char str = ch;
            //sendto(clientSock, &str, strlen(&str),NULL,&clntAddr,sizeof(struct sockaddr));
            send(clientSock, &str, strlen(&str)+sizeof(char), NULL);
        }
    }
        
 
 
 
 
    //关闭套接字
    closesocket(clientSock);
    closesocket(serverSock);
 
 
    //终止 DLL 的使用
    WSACleanup();
    system("pause");
 
    return 0;
}