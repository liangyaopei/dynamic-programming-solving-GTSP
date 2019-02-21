#include<iostream>
#include<fstream>
#include<time.h>
#include"Graph.h"
#include"heap.h"
using namespace std;

int main()
{
	int nodes = 1971280 + 2;
	Graph graph(nodes);
	int i;
	int from, to, weight;
	ifstream inFile("C:/Users/10305/Desktop/roadNet-CA.txt/roadNet.txt", ios::in);

	int src = graph.getSource();
	int des = graph.getDestination();

	while (!inFile.eof())
	{
		inFile >> from;
		inFile >> to;
		weight = rand() % 499 + 1;
		/*if (from == src)
			cout << from << ' ' << to << ' ' << weight << endl;
		if (to == des)
			cout << from << ' ' << to << ' ' << weight << endl;*/
		graph.addEdge(from, to, weight);
	}
	cout << "读取数据完毕" << endl;
	
	
	clock_t start,end;
	start = clock();
	graph.findPath();
	cout << endl;
	end = clock();
	cout << "运行时间是:" << end - start << endl;
	
	
	system("pause");
}