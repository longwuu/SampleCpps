#include<iostream>
#include<conio.h>
#include<String>
#include<windows.h>
#include <stdlib.h>
#include <time.h>
#include<vector>
using namespace std;

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
    Node* head;
    Node* last;
    int Length;
    int InitLength;
    Player(){
        head = new Node();
        last = head;
        Length = 0;
        InitLength = 0;
    }

    void init(int length){
        Length = length;
        InitLength = length;
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
    void ReStart(){
        Node* p = head->next;
        head->next = nullptr;
        last = head;
        while (p)
        {
            Node*Temp = p;
            p = p->next;
            delete(Temp);
        }
        init(InitLength);
    }
};
class Map{
    int Max = 10;
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
            map[k][Max] = 'I';
        }
        for(int k = 0;k < Max;k++){
            map[Max][k] = 'T';
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
                cout<< "\'R\' Restart"<<endl<<"\'R\' 重新开始";
                return;
            }
            map[L->y][L->x] = 'O';
        }
        map[L->y][L->x] = '8';
        map[Seed_x][Seed_y] = 'S';

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

int main(){
    char ch = ' ';
    Player player;
    player.init(5);
    Map SankeMap;
    //没有帧缓存，地图设太大看不清了就
    SankeMap.initMap(20);
    while(true){
        if(_kbhit()){
            ch = _getch();
        }
        switch (ch)
        {
        case 'd':
        case 'D':
            player.MoveX(1);
            break;
        case 's':
        case 'S':
            player.MoveY(1);
            break;
        case 'a':
        case 'A':
            player.MoveX(-1);
            break;
        case 'w':
        case 'W':
            player.MoveY(-1);
            break;
        case 'r':
        case 'R':
            player.ReStart();
            ch = ' ';     
            break;
        default:
            break;
        }
        if(SankeMap.IsOnSeed(player.last->x,player.last->y)){
            player.Add();
        }
        SankeMap.RenderFrame(player.head);
        cout<<player.Length<<endl;
        Sleep(100);
        system("CLS");
    }
    return 0;
}