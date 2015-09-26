#include "snake.h"
#include <fstream>
//#include <vector>
#include <queue>
#include <sstream>


//bool isStart = true;
vector<string> log;

vector<vector<int> > map;
queue<int> moveQueue;
//bool roundFinish=false;
int headtoTail=-1;
int loopTimes=0;
bool HasGoAround=false;

bool except=false;

void autoMove(Snake*,int,int,int,int );
void direct_Mode(Snake* ,int,int,int,int);
void initial_Mode(Snake* ,int,int,int,int);
void trackTail_Mode(Snake* ,int,int);
bool goAround_Mode(Snake* s,int maxX,int maxY);

void push(Snake* ,queue<int>* ,int );
void pushUP(Snake* ,queue<int>*);
void pushDOWN(Snake* ,queue<int>*);
void pushLEFT(Snake* ,queue<int>*);
void pushRIGHT(Snake* ,queue<int>*);
void printLog(vector<string>);

void snakeToMap(Snake* ,int ,int );
bool checkSnake(Snake* ,int,int);
void TagDistance(int ,int ,int ,int ,int );
int calDistance(Snake* ,int,int,int,int);
int findmin(vector<int>);
bool canGoTail(Snake* ,int maxX,int maxY);
bool canGoFoodnoTail(Snake* s,int maxX,int maxY);
string int2str(int );

//主方法
void autoMove(Snake* s,int tarX,int tarY,int maxX,int maxY){
    //當佇列為空時才會動作
    /*if(loopTimes>5 )
    {
        log.push_back("fall into infinte loop, Run go Around Mode ");
        bool tmp=goAround_Mode(s,maxX,maxY);
        if(tmp==true){
            log.push_back("RunAround Mode Success");
            loopTimes=0;
            //except = false;
        }
    }*/
    if(s->head->x==s->tail->x &&s->head->y==s->tail->y)
        log.push_back("head and tail is same");
    while(moveQueue.size()==0)
    {
        //log.push_back("initial Mode was called in autoMove");
        initial_Mode(s,tarX,tarY,maxX,maxY);
        printLog(log);
    }
    log.push_back("========================================");
    printLog(log);
}
//將蛇方向設定的模式
void initial_Mode(Snake* s,int tarX,int tarY,int maxX,int maxY){
    vector<int>v;
    //使用三層樹判斷初步方向的決定
    int first =-1;
    int second=-1;
    int third =-1;
    //最小值
    int before=calDistance(s,tarX,tarY,maxX,maxY);;
    int min   =100000;
    bool stop =false;//表示這次loop無效，
    bool find =false;//表示已經找到食物的路徑
    Snake* newSnake = new Snake;
    newSnake->operator=(s);

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                if(find==false){
                    stop=false;
                    Snake* g = new Snake;
                    g->operator=(s);
                    int after;//存放距離
                    //i
                    if(stop==false && find==false){
                        stop=!g->ghostMove(258+i,maxX,maxY);
                    }
                    if(calDistance(g,tarX,tarY,maxX,maxY)==0 && stop==false &&find==false && calDistance(g,g->tail->x,g->tail->y,maxX,maxY)>=1){
                        first=i;
                        second=-1;
                        third =-1;
                        find=true;
                        newSnake->operator=(g);
                    }
                    //j
                    if(stop==false &&find==false){
                        stop=!g->ghostMove(258+j,maxX,maxY);
                    }
                    if(calDistance(g,tarX,tarY,maxX,maxY)==0 && stop==false &&find==false&& calDistance(g,g->tail->x,g->tail->y,maxX,maxY)>=1){
                        first=i;
                        second=j;
                        third =-1;
                        find=true;
                        newSnake->operator=(g);
                    }
                    //k
                    if(stop==false &&find==false){
                        stop=!g->ghostMove(258+k,maxX,maxY);
                    }
                    if(calDistance(g,tarX,tarY,maxX,maxY)==0 && stop==false &&find==false&& calDistance(g,g->tail->x,g->tail->y,maxX,maxY)>=1){
                        first=i;
                        second=j;
                        third =k;
                        find=true;
                        newSnake->operator=(g);
                    }
                    
                    after=calDistance(g,tarX,tarY,maxX,maxY);
                    //未找到但靠近中
                    if(after<min && find == false && stop==false && after>0)
                    {
                        min=after;
                        first=i;
                        second=j;
                        third=k;
                        newSnake->operator=(g);
                    }
                }
            }
        }
    }
    
    log.push_back("The result of the initial Mode:");
    log.push_back("find= "+ int2str(find));
    log.push_back("before is " +int2str(before));
    log.push_back("after is " + int2str(min));
    log.push_back("decision is " + int2str(first) + int2str(second) + int2str(third));
    int a =calDistance(s,s->tail->x,s->tail->y,maxX,maxY);
    log.push_back("the between head and tail " + int2str(a));
    
    if(calDistance(s,s->tail->x,s->tail->y,maxX,maxY)==-1)
    {
        log.push_back("Warning! Warning! Warning!");
    }
    //原本就和食物無連接
    else if(before==-1)
    {
        log.push_back("snake's head isn't connect with food");
        printLog(log);
        trackTail_Mode(s,maxX,maxY);
    }
    //新產生的佇列，使得頭和尾無法連接，追尾
    else if(calDistance(newSnake,newSnake->tail->x,newSnake->tail->y,maxX,maxY)==-1)
    {
        log.push_back("the result of initialMode made the head and tail not connect");
        trackTail_Mode(s,maxX,maxY);
    }
    //此次選擇是合法的，加入佇列
    else
    {
        log.push_back("Success");
        push(s,&moveQueue,first);
        push(s,&moveQueue,second);
        push(s,&moveQueue,third);
        //except=false;
        
    }
}
//追尾模式
void trackTail_Mode(Snake* s,int maxX,int maxY){
    log.push_back("run tracktail mode");
    printLog(log);
    
    int tarX=s->tail->x;
    int tarY=s->tail->y;
    int before = calDistance(s,tarX,tarY,maxX,maxY);
    int after ;
    //使用三層樹判斷初步方向的決定
    int first =-1;
    int second=-1;
    int third =-1;
    Snake* g = new Snake;
    //最小值
    int max=-1;
    bool stop =false;//表示這次loop無效，
    //bool find =false;//表示已經找到食物的路徑
    Snake* n= new Snake;
    n->operator=(s);
    for(int i=0;i<4;i++){
        for(int j=0;j<1;j++){
            for(int k=0;k<1;k++){
                g->operator=(s);
                //
                stop=!g->ghostMove(258+i,maxX,maxY);
                after = calDistance(g,g->tail->x,g->tail->y,maxX,maxY);
                if(stop==false && after!=-1 && (after>max||max==-1))
                {
                    max = after;
                    first=i;
                    second=-1;
                    //third=-1;
                    n->operator=(g);
                }
                //
                /*
                stop=!g->ghostMove(258+j,maxX,maxY);
                after = calDistance(g,g->tail->x,g->tail->y,maxX,maxY);
                if(stop==false && after!=-1 && (after>max||max==-1))
                {
                    max = after;
                    first=i;
                    second=j;
                    //third=-1;
                    n->operator=(g);
                }*/
                
            }
        }
    }
    int a =calDistance(n,n->tail->x,n->tail->y,maxX,maxY);
    if(a==-1)
    {
        log.push_back("Warning! Warning! Warning!");
        log.push_back("max is "+int2str(max));
        
        log.push_back("the between head and tail is"+int2str(a));
        log.push_back("false,decision is "+ int2str(first) + int2str(second) + int2str(third));
    }
    else if(first !=-1 || second!=-1 || third!=-1)
    
    {
        log.push_back("Success,decision is "+ int2str(first) + int2str(second) + int2str(third));
        push(s,&moveQueue,first);
        //push(s,&moveQueue,second);
        //push(s,&moveQueue,third);
    }
    else
    {
        log.push_back("the between head and tail is"+int2str(a));
        
        log.push_back("false,decision is "+ int2str(first) + int2str(second) + int2str(third));
    }
    
    free(n);
    free(g);
    
}
//繞畫面
bool goAround_Mode(Snake* s,int maxX,int maxY){
    log.push_back("Run goaround Mode");
    int tailX=s->tail->x;
    int tailY=s->tail->y;
    
    snakeToMap(s,maxX,maxY);
    Snake* g=new Snake;
    bool r1 = false;
    bool r2 = false;
    
    int max=0;
    max = calDistance(s,tailX,tailY,maxX,maxY);
    int first=-1;
    int second=-1;
    int third = -1;
    bool tmp =false;
    int d;
    
    for(int i = 0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                
                g->operator=(s);
                tmp = g->ghostMove(i+258,maxX,maxY);
                d=calDistance(g,tailX,tailY,maxX,maxY);
                if(tmp==true && d>max)
                {
                    //max=d;
                    first=i;
                    r1 = true;
                }
                
                tmp =g->ghostMove(j+258,maxX,maxY);
                d=calDistance(g,tailX,tailY,maxX,maxY);
                if(tmp==true && d>max && r1 == true )
                {
                    //max=d;
                    second=j;
                    r2 = true;
                }
                
                tmp =g->ghostMove(k+258,maxX,maxY);
                d=calDistance(g,tailX,tailY,maxX,maxY);
                if(tmp==true && d>max && r2 == true )
                {
                    //max=calDistance(g,tailX,tailY,maxX,maxY);
                    third=k;
                }
            }
        }
    }
    if(r1==true && d!=-1){
        log.push_back("the result of goaround is true");
        log.push_back("decision is " + int2str(first) + int2str(second) + int2str(third));
        
        push(s,&moveQueue,first);
        push(s,&moveQueue,second);
        push(s,&moveQueue,third);
    }
    else
        log.push_back("the result of goaround is false");
    return r1;
        
}
//對map標記可行空間和距離,xy分別是中心的位置,d是中心的距離
void TagDistance(int x,int y,int d,int maxX,int maxY){
    //下方
    if(y+1<maxY){
        if(map[y+1][x]==-1 || (map[y+1][x]!=0 && map[y+1][x]>d+1)){
            map[y+1][x]=d+1;
            TagDistance(x,y+1,d+1,maxX,maxY);
        }
    }
    //上方
    if(y-1>=0){
        if(map[y-1][x]==-1 || (map[y-1][x]!=0 && map[y-1][x]>d+1)){
            map[y-1][x]=d+1;
            TagDistance(x,y-1,d+1,maxX,maxY);
        }
    }
    //左方
    if(x-1>=0){
        if(map[y][x-1]==-1 || (map[y][x-1]!=0 && map[y][x-1]>d+1)){
            map[y][x-1]=d+1;
            TagDistance(x-1,y,d+1,maxX,maxY);
        }
    }
    //右方
    if(x+1<maxX){
        if(map[y][x+1]==-1 || (map[y][x+1]!=0 && map[y][x+1]>d+1)){
            map[y][x+1]=d+1;
            TagDistance(x+1,y,d+1,maxX,maxY);
        }
    }
}
//將蛇印上map
void snakeToMap(Snake* s,int maxX,int maxY){
    //初始化map
    map.resize(maxY);
    for(int i =0;i<maxY;i++){
        map[i].resize(maxX);
    }
    for(int i=0;i<maxY;i++){
        for(int j=0;j<maxX;j++)
        {
            map[i][j]=-1;
        }
    }
    s->head->prevNode=NULL;
    Node* n = s->tail;
    while(n->prevNode!=NULL)
    {
        map[n->y][n->x]=0;
        n=n->prevNode;
    }
    map[n->y][n->x]=0;
}
//計算距離checkSnake(s,maxX,maxY)==false
int calDistance(Snake* s,int tarX,int tarY,int maxX,int maxY){
    int result=0;
    
    if(abs(s->CurX()-tarX)==1 && s->CurY()==tarY)
        return 1;
    else if(abs(s->CurY()-tarY)==1 && s->CurX()==tarX)
        return 1;
    else if(s->CurX()==tarX && s->CurY()==tarY)
        return 0;//到達目標
    else if(s->head->x>=maxX || s->head->y>=maxY || s->head->x<0 ||s->head->y<0 )
        return -1;//越界
    else{
        snakeToMap(s,maxX,maxY);
        TagDistance(s->CurX(),s->CurY(),0,maxX,maxY);
        if(map[tarY][tarX]!=0)
            return map[tarY][tarX];
        else{
            vector<int>v;
            if(tarX+1<maxX)
                v.push_back(map[tarY][tarX+1]);
            if(tarX-1>=0)
                v.push_back(map[tarY][tarX-1]);
            if(tarY-1>=0)
                v.push_back(map[tarY-1][tarX]);
            if(tarY+1<maxY)
                v.push_back(map[tarY+1][tarX]);
            result = findmin(v);
            return result;
        }
    }
}
//佇列處理
void push(Snake* s,queue<int>* q,int i){
    switch(i)
    {
        case 0:
            pushDOWN(s,q);
            break;
        case 1:
            pushUP(s,q);
            break;
        case 2:
            pushLEFT(s,q);
            break;
        case 3:
            pushRIGHT(s,q);
            break;
        case -1:
            break;
    }
}
void pushUP(Snake* s,queue<int>* q){
    if(s->dir!=DOWN)
        q->push(259);
    //else
        //q->push(0);
}
void pushDOWN(Snake* s,queue<int> *q){
    if(s->dir!=UP)
        q->push(258);
    //else
        //q->push(0);
}
void pushLEFT(Snake* s,queue<int> *q){
    if(s->dir!=RIGHT)
        q->push(260);
    //else
        //q->push(0);
}
void pushRIGHT(Snake* s,queue<int> *q){
    if(s->dir!=LEFT)
        q->push(261);
    //else
        //q->push(0);
}
void printLog(vector<string> v){
    ofstream file("log");
    
    if (!file) return;
    
    for(int i=0;i<v.size();i++)
    {
        file<<v[i]<<"\n";
        //if(v[i]==1)
        //file<<"\n\n";
    }
    //file<<endl;
    file.close();
}

void printLog(vector<string> v,string name){
    ofstream file(name);
    
    if (!file) return;
    
    for(int i=0;i<v.size();i++)
    {
        file<<v[i]<<"\n";
        //if(v[i]==1)
        //file<<"\n\n";
    }
    //file<<endl;
    file.close();
}
int findmin(vector<int>v){
    int tmp = 100000;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]>0 && v[i]<tmp)
            tmp=v[i];
    }
    if(tmp==100000)
        return -1;
    else return tmp+1;
}
/*bool canGoTail(Snake* s,int maxX,int maxY){
    int x = s->tail->x;
    int y = s->tail->y;
    if(calDistance(s,x,y,maxX,maxY)!=-1)
    {
        return true;
    }
    else return false;
}
bool canGoFoodnoTail(Snake* s,int maxX,int maxY){
    snakeToMap(s,maxX,maxY);
    TagDistance(s->CurX(),s->CurY(),0,maxX,maxY);
    int i=0;
    int j=0;
    for(int i=0;i<map.size();i++)
    {
        for(int j=0;j<map[0].size();j++)
        {
            if(map[i][j]==-1)
            {
                i++;
            }
            else if(map[i][j]>0)
            {
                j++;
            }
        }
    }
    if(j>i)
        return true;
    else return false;
}*/
string int2str(int i) {
      string s;
      stringstream ss(s);
      ss << i;
    
      return ss.str();
    }
string dbl2str(double i) {
    string s;
    stringstream ss(s);
    ss << i;
    
    return ss.str();
}













