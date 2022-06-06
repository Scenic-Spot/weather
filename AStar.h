#ifndef ASTAR_H
#define ASTAR_H

#endif // ASTAR_H
#pragma once
#include<list>
#include<vector>

using namespace std;

const int kCost1 = 10;//直移一格消耗
const int kCost2 = 14;//斜移一格消耗

typedef struct _Point
{
    int x, y;
    int F, G, H;//F=G+H
    struct _Point *parent;
}Point;

/*分配一个格子*/
Point *AllocPoint(int x, int y);
/*初始化地图*/
void InitAstarMaze(int *Maze, int lines, int colums);
/*A*算法寻找路径*/
list<Point*> GetPath(Point *startPoint, Point *endPoint);
/*清理资源*/
void ClearAstarMaze();

