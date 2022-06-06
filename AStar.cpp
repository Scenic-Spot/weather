#include<math.h>
#include"AStar.h"
#include <iostream>
#include <vector>
#include <string.h>

#include "PathList.h"
#include "Path.h"
static int *maze;//地图
static int cols;//列数
static int lines;//行数

static list<Point*>openList;
static list<Point*>closeList;

/*A*算法寻找路径*/
Point * isInList(const list<Point *> point, const Point *target);
static Point *GetLeastPoint();//找出openlist中F值最小的节点
static vector<Point*> GetSurroundPoints(const Point *point);//找到当前结点周围可达的节点
bool isCanreach(const Point *point, const Point *target);//判断这两点是否可达
int calcDistance(Point *target, Point *endPoint);

static Point *FindPath(Point *startPoint, Point *endPoint)
{
    openList.push_back(AllocPoint(startPoint->x,startPoint->y));

    while (!openList.empty())
    {
        //1.第一步 取openlist中的F最小值
        Point *curPoint = GetLeastPoint();

        //2.将当前结点加入到closelist中
        openList.remove(curPoint);
        closeList.push_back(curPoint);

        //3.找到当前结点周围可达的节点 并计算F值
        vector<Point *>surroundPoints = GetSurroundPoints(curPoint);
        for (vector<Point *>::const_iterator it= surroundPoints.begin();it!= surroundPoints.end();it++)
        {
            Point *target = *it;
            Point *exsit = isInList(openList, target);
            if (!exsit)
            {
                target->parent = curPoint;
                target->G = calcDistance(curPoint, target);
                target->H = calcDistance(target, endPoint);
                target->F = target->G + target->H;
                openList.push_back(target);
            }
            else
            {
                int tmpG = calcDistance(curPoint, target);
                if (tmpG<target->G)
                {
                    exsit->parent = curPoint;
                    exsit->G = tmpG;
                    exsit->F = tmpG + exsit->H;
                }

                delete target;
            }
        }
        surroundPoints.clear();
        Point *resPoint = isInList(openList, endPoint);
        if (resPoint)
            return resPoint;
    }

}

void InitAstarMaze(int *arr,int _lines, int colums)//地图初始化
{
    maze = arr;
    cols = colums;
    lines = _lines;
}

Point * AllocPoint(int x, int y)//分配格子
{
    Point * tmp = new Point;
    memset(tmp, 0, sizeof(Point));
    tmp->x = x;
    tmp->y = y;
    return tmp;
}

/*返回正确路径*/
list<Point*> GetPath(Point *startPoint, Point *endPoint)
{
    Point *result = FindPath(startPoint, endPoint);

    list<Point*> path;

    //返回路径，如果没有找到路径则返回空链表

    while (result)
    {
        path.push_front(result);
        result = result->parent;
    }

    return path;
}

static Point *GetLeastPoint()//找出openlist中F值最小的节点
{
    Point *resPoint = openList.front();
    if (!openList.empty())
    {

        for (list<Point *>::const_iterator it= openList.begin();it!=openList.end();it++)

            if ((*it)->F < resPoint->F)
            {
                resPoint = *it;
            }
    }

    return resPoint;
}

static vector<Point*> GetSurroundPoints(const Point *point)//找到当前节点可达节点
{
    vector<Point *> surroundPoints;

    for (int x= point->x-1;x<= point->x+1;x++)
        for (int y = point->y - 1; y <= point->x+1; y++)
    {
            Point *tmp = AllocPoint(x, y);
            if (isCanreach(point,tmp))
            {
                surroundPoints.push_back(tmp);
            }
            else
            {
                delete tmp;
            }
    }
    return surroundPoints;
}

Point * isInList(const list<Point *> point,const Point *target)
{
    for (list<Point *>::const_iterator it = point.begin(); it != point.end(); it++)
    {
        if ((*it)->x==target->x && (*it)->y==target->y)
        {
            return *it;
        }
    }
    return NULL;
}
bool isCanreach(const Point *point, const Point *target)//判断这两点是否可达
{
    if (target->x<0|| target->x>(lines-1)
        || target->y<0|| target->y>(cols-1)
        || maze[target->x*cols+target->y]==1//1代表无法走的格子
        || maze[target->x*cols + target->y] == 2//2代表无法走的格子
        || (target->x==point->x && target->y==point->y)
        || isInList(closeList,target)!=NULL)
    {
        return false;
    }

    if (abs(point->x - target->x) + abs(point->y - target->y) == 1)
        return true;
    else
        return false;
}


int calcDistance(Point *target, Point *endPoint)//计算两点间距离（H）
{
    return abs(target->x - endPoint->x) + abs(target->y - endPoint->y);
}

void ClearAstarMaze()//回收相应资源
{
    maze = NULL;
    lines = cols = 0;
    for (list<Point *>::iterator it=openList.begin();it!=openList.end();)
    {
        delete *it;
        it = openList.erase(it);
    }

    for (list<Point *>::iterator it = closeList.begin(); it != closeList.end();)
    {
        delete *it;
        it = closeList.erase(it);
    }
}


int _map[48][38] = {//将地图网格化为14*14像素大小的格子
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1 ,0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

};

    void A(){//main函数改编，相当于函数入口
        //要与景点绑定，也要与点击事件绑定
        InitAstarMaze(&_map[0][0], 48, 38);
        Point *start = AllocPoint(12, 12);
        Point *_end = AllocPoint(0, 0);
        list <Point*> path = GetPath(start, _end);

        for (list<Point*>::iterator it = path.begin(); it != path.end(); it++)
        {
            Point *res = *it;
            //cout << "(" << res->x << "," << res->y << ")" << endl;
            //不用输出
        }
        ClearAstarMaze();

    }
