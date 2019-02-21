#include<iostream>
#include<time.h>
#include<math.h>
#include"Graph.h"
#include"heap.h"
using namespace std;

Graph::Graph(int VexNum)
{
	vexnum = VexNum;
	adj = new GNode*[vexnum];
	int i,j;
	for (i = 0; i < vexnum; ++i)
	{
		adj[i] = new GNode();
	}
	source = rand();
	destination = rand();
	cout << "源点:" << source << ",目标节点:" << destination << endl;


	queryLen = 3;
	query = new int[queryLen];//查询3个点
	cout << "要查询的点:";
	for (i = 0; i < queryLen; ++i)
	{
		query[i] = rand();
		cout << query[i] << ' ';
	}
	cout << endl;

	int totalPath = (queryLen + 2)*(queryLen + 1);//n个点共有n*n(-1)条边
	pathStack = new stack<int>[totalPath+1];
	currentPath = 1;//从1开始计数

	arc = new int*[queryLen + 2];//初始化邻接矩阵
	for (i = 0; i < queryLen + 2; ++i)
	{
		arc[i] = new int[queryLen + 2];
		arc[i][i] = INT_MAX;
	}
	vertexThrough = 0;
	runTime = 1;
}


//尾接法加边
void Graph::addEdge(int from, int to, int w)
{
	GNode* node = new GNode();
	node->vex = to;
	node->weight = w;
	node->next = adj[from]->next;
	adj[from]->next = node;
}




//利用BFS和迪杰斯特拉算法思想的结合找最短路径
int Graph::BFS(int src, int des)
{
	int capacity = 1971280;
	Heap heap(capacity,src,des);
	bool* visit = new bool[vexnum];//记录当前节点是否被访问过
	int* path = new int[vexnum]; //记录节点的前驱

	GNode* p = adj[src]->next;//源点的邻边
	int i = 0;
	for (; i < vexnum; ++i)//初始化
	{
		visit[i] = false;
		path[i] = -1;
	}
		
	//将与源点直接相邻的点入堆,标记已经访问
	for (; p != NULL; p = p->next)
	{
		heap.insert2(p->vex, p->weight);
		visit[p->vex] = true;
	}
	path[src] = src;
	//heap.buildMinHeap();

	HNode node;
	i = 0;
	while (!heap.isEmpty())
	{
		node = heap.extractMin();
		visit[node.id] = true;//将取出来的点标记为已经访问，防止成环

		if (node.id == des)//如果取出的点是目标节点，跳出循环
			break;

		int dist = node.dist;//获得当前距离

		p = adj[node.id]->next;//寻找邻边
		for (; p != NULL; p = p->next)
		{
			if (!visit[p->vex])//防止成环
			{
				heap.insert2(p->vex, p->weight + dist);//插入的是当前节点的权重加上上一条边的距离
				path[p->vex] = node.id; //记录当前节点的前驱节点
			}
				
		}
		//heap.buildMinHeap();
	}
	
	//cout << src << " to " << des << ",dist is:" << node.dist << endl;
	//cout << "path" << endl;
	
	
	i = node.id;//从目标节点向前找前驱，获得最短路径
	pathStack[currentPath].push(i);//放入目标节点
	while (path[i] != -1)
	{
		pathStack[currentPath].push(path[i]);
		++vertexThrough;
		//cout << path[i] << ' ';
		i = path[i]; //找下一个前驱
	}
	pathStack[currentPath].push(src);//放入源点

	pathStack[currentPath].push(des);//放入一个目标节点，做标记
	//cout << endl;

	++currentPath;//当前路径+1

	delete[] visit;
	delete[] path;

	return node.dist;//返回距离
}



void Graph::initArc()
{
	//源点
	int srcToQ0 = BFS(source, query[0]);
	int srcToQ1 = BFS(source, query[1]);
	int srcToQ2 = BFS(source, query[2]);
	int srcToDes = BFS(source, destination);
	arc[0][1] = srcToQ0;
	arc[0][2] = srcToQ1;
	arc[0][3] = srcToQ2;
	arc[0][4] = srcToDes;

	//点0
	int q0ToSrc = BFS(query[0], source);
	int q0ToQ1 = BFS(query[0], query[1]);
	int q0ToQ2 = BFS(query[0], query[2]);
	int q0ToDes = BFS(query[0], destination);
	arc[1][0] = q0ToSrc;
	arc[1][2] = q0ToQ1;
	arc[1][3] = q0ToQ2;
	arc[1][4] = q0ToDes;

	//点1
	int q1ToSrc = BFS(query[1], source);
	int q1ToQ0 = BFS(query[1], query[0]);
	int q1ToQ2 = BFS(query[1], query[2]);
	int q1ToDes = BFS(query[1], destination);
	arc[2][0] = q1ToSrc;
	arc[2][1] = q1ToQ0;
	arc[2][3] = q1ToQ2;
	arc[2][4] = q1ToDes;

	//点2
	int q2ToSrc = BFS(query[2], source);
	int q2ToQ0 = BFS(query[2], query[0]);
	int q2ToQ1 = BFS(query[2], query[1]);
	int q2ToDes = BFS(query[2], destination);
	arc[3][0] = q2ToSrc;
	arc[3][1] = q2ToQ0;
	arc[3][2] = q2ToQ1;
	arc[3][4] = q2ToDes;

	//目标节点
	int desToSrc = BFS(destination, source);
	int desToQ0 = BFS(destination, query[0]);
	int desToQ1 = BFS(destination, query[1]);
	int desToQ2 = BFS(destination, query[2]);
	arc[4][0] = desToSrc;
	arc[4][1] = desToQ0;
	arc[4][2] = desToQ1;
	arc[4][3] = desToQ2;

	
	
}

//n个节点的表，有n行，2^(n-1)列
void Graph::findPath()
{
	clock_t start, end;
	start = clock();
	//initArc();
	anotherInitArc();
	end = clock();
	cout << "耗时:" << end - start << "毫秒" << endl;
	
	int len = queryLen + 2;//行数
	int col = pow(2,len-1);//，列数，2的n-1次方列

	int i, j, k,min,temp,route;//用到的变量

	int** dist = new int*[len];  //记录距离
	int** mark = new int*[len]; //记录路径
	for (i = 0; i < len; ++i)
	{
		dist[i] = new int[col];
		mark[i] = new int[col];
	}
	for (i = 0; i < len; ++i)
	{
		for (j = 0; j < col; ++j)
		{
			dist[i][j] = -1;
			mark[i][j] = -1;
		}
	}

	//对dist的第0列赋值
	for (i = 0; i < len; ++i)
		dist[i][0] = arc[i][0];

	

	for (i = 1; i < col - 1; ++i)//最后一列不在循环里计算
	{
		for (j = 1; j < len; ++j)
		{
			if (((int)pow(2, j - 1) & i) == 0)//结点j不在i表示的集合中
			{
				min = INT_MAX;
				for (k = 1; k < len; ++k)
				{
					if ((int)pow(2, k - 1) & i)//非零表示结点k在集合中  
					{
						temp = arc[j][k] + dist[k][i - (int)pow(2, k - 1)];
						if (temp < min)
						{
							min = temp;
							dist[j][i] = min; //阶段最优值
							mark[j][i] = k;   //保存最优决策
						}
					}
				}
			}
		}
	}

	//计算最后一列,计算中最优值  
	min = INT_MAX;
	for (k = 1; k < len; ++k)
	{
		//b-1的二进制全1，表示集合{1,2,3,4,5}，从中去掉k结点即将k对应的二进制位置0  
		temp = arc[0][k] + dist[k][col - 1 - (int)pow(2, k - 1)];
		if (temp < min)
		{
			min = temp;
			dist[0][col - 1] = min;//总最优解  
			mark[0][col - 1] = k;
		}
	}

	/*for (i = 0; i < len; ++i)
	{
		for (j = 0; j < len; ++j)
		{
			if (arc[i][j] != INT_MAX)
				cout << arc[i][j] << ' ';
			else
				cout << "INF" << ' ';
		}
		cout << endl;
	}*/
	
	cout << "源点:" << source << ",目标节点:" << destination << endl<<"目标节点:";
	for (i = 0; i < 3; ++i)
		cout << query[i] << ' ';
	cout << "最短路径长度:" << dist[0][col - 1] << endl;

	cout << "最短路径:"<<source;
	int result[5] = { source,query[0],query[1],query[2],destination };

	int pathOrder[6];
	int cur = 0;
	pathOrder[cur++] = 0;//0对应源点
	for (i = col - 1, j = 0; i>0; )//i的二进制是5个1，表示集合{1,2,3,4,5}  
	{
		j = mark[j][i];//下一步去往哪个结点  
		i = i - (int)pow(2, j - 1);//从i中去掉j结点  
		cout << "->" << result[j];
		pathOrder[cur++] = j;//对应的下标
	}
	cout << "->" << result[0] << endl;
	pathOrder[cur++] = 0;
	

	cout << "经过的节点数:" << vertexThrough << endl;//输出经过的节点数

	j = 0;
	int count=0;
	int PRE;
	int next;
	int index;
	int valid;//校验
	for (i=0; i<cur-2; ++i)
	{
		PRE = pathOrder[i];
		next = pathOrder[i + 1];
	
		cout << "from " << result[PRE] << " to " << result[next] <<". distance:"<<arc[PRE][next]<< endl;
		index = 4 * PRE;
		if (index == 0)
			index += 1;
		
		if (index == 4)
			index += 1;
	
		valid = pathStack[index].top();
		while (valid != result[next])//验证目标节点的标记
		{
			++index;
			if (!pathStack[index].empty())
				valid = pathStack[index].top();
		}
		pathStack[index].pop();

		while (!pathStack[index].empty())
		{
			route = pathStack[index].top();
			pathStack[index].pop();
			cout << route << "->";
			++count;
			if (count % 10 == 0)
				cout << endl;
		}
		cout << endl;
	}
	
	//输出从最后一个节点到源点的路径
	index = next * 4;
	for (; pathStack[index].empty(); ++index)
		;

	valid = pathStack[index].top();
	while (valid != result[0])//验证目标节点的标记
	{
		++index;
		if (!pathStack[index].empty())
			valid = pathStack[index].top();
	}
	pathStack[index].pop();

	cout << endl << "back  to Source:" << "form:" << result[pathOrder[i]] << " to " << result[0] << endl;;
	while (!pathStack[index].empty())
	{
		route = pathStack[index].top();
		pathStack[index].pop();
		cout << route << "->";
		++count;
		if (count % 10 == 0)
			cout << endl;
	}
	cout << endl;
	
}


void Graph::anOtherBFS(int src, int* arr,int* result)
{
	int capacity = 1971280;
	Heap heap(capacity, src, arr[0]);
	bool* visit = new bool[vexnum];//记录当前节点是否被访问过
	int* path = new int[vexnum]; //记录节点的前驱

	GNode* p = adj[src]->next;//源点的邻边
	int i = 0;
	for (; i < vexnum; ++i)//初始化
	{
		visit[i] = false;
		path[i] = -1;
	}

	//将与源点直接相邻的点入堆,标记已经访问
	for (; p != NULL; p = p->next)
	{
		heap.insert2(p->vex, p->weight);
		visit[p->vex] = true;
	}
	path[src] = src;
	//heap.buildMinHeap();

	int arrCopy[4];
	for (i = 0; i < 4; ++i)
		arrCopy[i] = arr[i];

	HNode node;
	i = 0;
	int count = 0;
	while (!heap.isEmpty())
	{
		node = heap.extractMin();
		visit[node.id] = true;//将取出来的点标记为已经访问，防止成环

		if (count < 4)
		{
			if (node.id == arr[0])
			{
				result[0] = node.dist;
				arr[0] = -1;
				++count;
			}
			else if (node.id == arr[1])
			{
				result[1] = node.dist;
				arr[1] = -1;
				++count;
			}
			else if (node.id == arr[2])
			{
				result[2] = node.dist;
				arr[2] = -1;
				++count;
			}
			else if (node.id == arr[3])
			{
				result[3] = node.dist;
				arr[3] = -1;
				++count;
			}
		}
		else
			break;

		int dist = node.dist;//获得当前距离

		p = adj[node.id]->next;//寻找邻边
		for (; p != NULL; p = p->next)
		{
			if (!visit[p->vex])//防止成环
			{
				heap.insert2(p->vex, p->weight + dist);//插入的是当前节点的权重加上上一条边的距离
				path[p->vex] = node.id; //记录当前节点的前驱节点
			}

		}
		//heap.buildMinHeap();
	}
	int j;
	for (j = 0; j < 4; ++j)
	{
		i = arrCopy[j];//从目标节点向前找前驱，获得最短路径
		

		pathStack[currentPath].push(i);//放入目标节点
		while (path[i] != -1)
		{
			pathStack[currentPath].push(path[i]);
			++vertexThrough;
			i = path[i]; //找下一个前驱
		}
		pathStack[currentPath].push(src);//放入源点

		pathStack[currentPath].push(arrCopy[j]);//放入一个目标节点，做标记
										

		++currentPath;//当前路径+1
	}

	delete[] path;
	delete[] visit;
}

void Graph::anotherInitArc()
{
	int nums[4];
	int result[4];
	int i = 0; 
	int j;

	nums[0] = query[0];
	nums[1] = query[1];
	nums[2] = query[2];
	nums[3] = destination;
	anOtherBFS(source, nums, result);
	arc[0][1] = result[0];
	arc[0][2] = result[1];
	arc[0][3] = result[2];
	arc[0][4] = result[3];

	//点0
	nums[0] = source;
	nums[1] = query[1];
	nums[2] = query[2];
	nums[3] = destination;
	anOtherBFS(query[0], nums, result);
	arc[1][0] = result[0];
	arc[1][2] = result[1];
	arc[1][3] = result[2];
	arc[1][4] = result[3];

	//点1
	nums[0] = source;
	nums[1] = query[0];
	nums[2] = query[2];
	nums[3] = destination;
	anOtherBFS(query[1], nums, result);
	arc[2][0] = result[0];
	arc[2][1] = result[1];
	arc[2][3] = result[2];
	arc[2][4] = result[3];

	//点2
	nums[0] = source;
	nums[1] = query[0];
	nums[2] = query[1];
	nums[3] = destination;
	anOtherBFS(query[2], nums, result);
	arc[3][0] = result[0];
	arc[3][1] = result[1];
	arc[3][2] = result[2];
	arc[3][4] = result[3];

	//目标节点
	nums[0] = source;
	nums[1] = query[0];
	nums[2] = query[1];
	nums[3] = query[2];
	anOtherBFS(destination, nums, result);
	arc[4][0] = result[0];
	arc[4][1] = result[1];
	arc[4][2] = result[2];
	arc[4][3] = result[3];
}

int Graph::getSource()
{
	return source;
}

int Graph::getDestination()
{
	return destination;
}

void Graph::setSource(int src)
{
	source = src;
}

void Graph::setDestination(int des)
{
	destination = des;
}

void Graph::setQuery()
{
	++runTime;
	source = source*runTime;
	destination = destination*runTime;
	query[0] = query[0] * runTime;
	query[1] = query[1] * runTime;
	query[2] = query[2] * runTime;
	vertexThrough = 0;
}