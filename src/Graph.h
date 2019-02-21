#pragma once
#ifndef GRAPH_H
#define GRAPH_H
#include<stack>
using namespace std;

class GNode
{
public:
	int vex;
	int weight;
	GNode* next;
	GNode()
	{
		vex = 0;
		next = NULL;
	}
};

class Graph
{
private:
	GNode** adj;
	int vexnum;

	int source;  //源点
	int destination; //目标节点

	int* query;//要找的集合
	int queryLen;//找3个集合
	int** arc;//邻接矩阵

	stack<int>* pathStack; //记录路径
	int currentPath;

	int vertexThrough;//经过的总节点数

	int BFS(int src, int des);//找从src到des的最短路径，返回距离
	void initArc(); //初始化邻接矩阵

	void anOtherBFS(int src, int* arr,int* result);
	void anotherInitArc();
	int runTime;
public:
	Graph(int v);
	void addEdge(int u, int v, int w);//邻接表加边
	

	
	void findPath();
	void setSource(int src);
	void setDestination(int des);
	int getSource();
	int getDestination();

	void setQuery();
};

#endif
