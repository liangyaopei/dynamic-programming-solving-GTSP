#include<iostream>
#include"heap.h"
using namespace std;


/*
初始化堆，下标从1开始
*/
Heap::Heap(int c,int s,int d)
{
	capacity = c;
	src = s;
	des = d;
	heap = new HNode[capacity+1];
	size = 0;
	heap[0].dist = -1;//不适用
	heap[0].id = des;
	for (int i = 1; i <= c; ++i)
	{
		heap[i].dist = INT_MAX;
		heap[i].id = i-1;  
	}
}

//取出最小值
HNode Heap::extractMin()
{
	//没有及诶单
	if (size == 0)
	{
		HNode node;
		node.id = des;
		node.dist = 0;
		return node;
	}
	HNode minItem = heap[1];
	HNode temp = heap[size--];//最后一个节点

	int pp;
	int parent, child;
	//调整最小堆
	for (parent = 1; parent * 2 <= size; parent = child)
	{
		child = parent * 2; //ȡ���ӽڵ��±�
		if ((child != size) && (heap[child].dist > heap[child + 1].dist))//ȡ��С�ĺ���
			++child;
		if (temp.dist <= heap[child].dist)//���������Ӷ�С
			break;
		else
		{
			heap[parent] = heap[child]; //��С�ĺ�����ǰ
		}
			
	}
	heap[parent] = temp;//��ǰ�ڵ�����λ��
	return minItem;
}

void Heap::buildMinHeap()
{
	int parent, child, i;
	HNode temp;
	for (i = size / 2; i > 0; --i)
	{
		temp = heap[i];
		for (parent = i; parent * 2 <= size; parent = child)
		{
			child = parent * 2;
			if ((child != size) && (heap[child].dist > heap[child+1].dist))
				++child;
			if (temp.dist <= heap[child].dist)
				break;
			else
				heap[parent] = heap[child];
		}
		heap[parent] = temp;
	}
}


//�������,ʱ�临�Ӷ���O(lgn)
//�����һ�����ڵ㿪ʼ������Ѱ�ң�ֱ�����ڵ�ľ���Ȳ���ľ���С���ߵ��˸��ڵ�(i=1)
//��Ϊ�Ǵ�1��ʼ���������Ը��׽ڵ���i/2,
void Heap::insert2(int key, int dist)
{
	++size;//���Ӽ���
	int i=size;
	HNode node;
	node.id = key;
	node.dist = dist;
	for (; i!=1 && heap[i / 2].dist > dist; i /= 2)//�븸�ڵ�Ƚϴ�С
	{
		heap[i] = heap[i / 2];//���ڵ��·�
	}	
	heap[i] = node;//��ǰ�ڵ�����λ��
}


Heap::~Heap()
{
	delete[] heap;
}

bool Heap::isEmpty()
{
	return size == 0;
}