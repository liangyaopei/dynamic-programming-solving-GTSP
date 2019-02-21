#pragma once
#ifndef HEAP_H
#define HEAP_H

//堆节点
class HNode
{
public:
	int id;  //图中的节点坐标
	int dist;  //当前节点与源点的距离
};

class Heap
{
private:
	HNode* heap;
	int size;
	int capacity;
	int src;  //源节点
	int des;  //目标节点
public:
	Heap(int c,int s,int d);
	~Heap();
	bool isEmpty();
	HNode extractMin();
	void buildMinHeap();
	void insert2(int id,int dist);
};

#endif