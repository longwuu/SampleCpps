#include<iostream>
#include<conio.h>
#include<String>
#include<windows.h>
#include <stdlib.h>
#include <time.h>
#include<mingw.thread.h>
#include <WinSock2.h>
#include<vector>
using namespace std;

//指向服务器IP
#define ServerIp "192.168.0.103"

#define LocalIP

#ifdef LocalIP
#define ServerIp "127.0.0.1"
#endif


class Node{
public:
    int x;
    int y;
    int prex;
    int prey;
    Node* next;
    Node* preNode;
    Node(){
        x = 0;
        y= 0;
        prex = 0;
        prey = 0;
        next = nullptr;
        preNode = nullptr;
    }
    Node(int x1,int y1,Node* N){
        x = x1;
        y = y1;
        prex = x1;
        prey = y1;
        next = N;
        preNode = nullptr;
    }
};

class Player{
public:
    int x_Max;
    int y_Max;
    Node* head;
    Node* last;
    int Length;
    Player(){
        x_Max = 0;
        y_Max = 0;
        head = new Node();
        last = head;
        Length = 0;
    }

    void init(int length){
        Length = length;
        Node* Temp = nullptr;
        for(int i = length - 1;i >= 0;i--){
            Temp = new Node(i,0,nullptr);
            Temp->next = head->next;
            Temp->preNode = head;
            if(head->next){
                head->next->preNode = Temp;
            }
            head->next = Temp;
        }
        while(last->next != nullptr){
            last = last->next;
        }
    }

    void Add(){
        Length++;
        Node* Temp = nullptr;
        Temp = new Node(0,0,nullptr);
        Temp->next = head->next;
        Temp->preNode = head;
        if(head->next){
            head->next->preNode = Temp;
        }
        head->next = Temp;
    }

    void MoveX(const int value){
        if(last != head){
            last->prex = last->x;
            last->prey = last->y;
            last->x += value;
        }
        UpdateNodes();
    }
    void MoveY(const int value){
        if(last != head){
            last->prex = last->x;
            last->prey = last->y;
            last->y += value;
        }
        UpdateNodes();
    }
    void UpdateNodes(){
        Node* p = last;
        while(p->preNode != nullptr && p->preNode->preNode != nullptr){
            p = p->preNode;
            p->prex = p->x;
            p->prey = p->y;
            p->x = p->next->prex;
            p->y = p->next->prey;
        }
      
    }
};
class Map{
    int Max = 20;
    char** map = nullptr;
    int Seed_x;
    int Seed_y;
public:
    //初始化地图
    void initMap(int size = 0){
        if(size > Max){
            Max = size;
        }
        //创建动态二维数组
        map = new char*[Max + 1];
        for(int i = 0;i <Max + 1;i++){
            map[i] = new char[Max + 1];
            for(int j = 0;j <Max;j++){
                map[i][j] = ' ';
            }
        }
        for(int k = 0;k < Max;k++){
            map[k][Max] = '2';
        }
        for(int k = 0;k < Max;k++){
            map[Max][k] = '2';
        }
        NewSeed();
    }
    int GetRadomNum(int min,int max){
        min++;
        max--;
        srand((unsigned)time(NULL)); 
        return rand()%(max - min) + min;
    }
    //释放内存
    void release(){
        for(int i = 0;i <Max;i++){
            delete map[i];
        }
        delete map;
    }
    void RenderFrame(Node* L){
        for(int i = 0;i <Max;i++){
            for(int j = 0;j <Max;j++){
                map[i][j] = ' ';
            }
        }
        while(L->next != nullptr){
            L = L->next;
            if(L->x >= Max || L->y >= Max || L->x < 0 || L->y < 0){
                cout<< "Error out of Map";
                return;
            }
            map[L->y][L->x] = '0';
        }

        map[Seed_x][Seed_y] = '1';

        for(int i = 0;i <Max + 1;i++){
            for(int j = 0;j <Max + 1;j++){
                cout<<map[i][j];
            }
            cout<<endl;
        }
    }
    void NewSeed(){
        Seed_x = GetRadomNum(0,Max);
        Seed_y = GetRadomNum(0,Max);
    }

    bool IsOnSeed(int x_pos,int y_pos){
        if(x_pos == Seed_x && y_pos == Seed_y){
            NewSeed();
            return true;
        }
        return false;
    }
};

void NetWorkFunc1(char* ch){
   
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ServerIp);
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //接收服务器传回的数据
    while(true){
        char szBuffer[MAXBYTE] = {0};
        recv(sock, szBuffer, MAXBYTE, NULL);
                
        if(*szBuffer == 'a' || *szBuffer == 'd'||*szBuffer == 'w'||*szBuffer == 's')
        {
            *ch = *szBuffer;
        }
    }
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
}
int main(){
    
    char ch = ' ';
    Player player;
    player.init(5);
    Map NewMap;
    NewMap.initMap(20);
    thread t1(NetWorkFunc1,&ch);
    t1.detach();
    while(true){

        if(ch == 'd' || ch == 'D'){
            player.MoveX(1);
        }
        if(ch == 's' || ch == 'S'){
            player.MoveY(1);
        }
        if(ch == 'a' || ch == 'A'){
            player.MoveX(-1);
        }
        if(ch == 'w' || ch == 'W'){
            player.MoveY(-1);
        }
        if(NewMap.IsOnSeed(player.last->x,player.last->y)){
            player.Add();
        }
        NewMap.RenderFrame(player.head);
        Sleep(100);
        system("CLS");
    }
 
    return 0;
}