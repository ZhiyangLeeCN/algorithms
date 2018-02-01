///////////////////////////////////////////////////////////////////////
// File:        main.cpp
// Description: Dijkstra's shortest Path Algorithm.
// Author:      ZhiyangLee
//
///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <vector>
#include <queue>
#include <algorithm>

using std::vector;
using std::queue;

#define VERTICES_SIZE 8
#define COL_SIZE VERTICES_SIZE
#define ROW_SIZE VERTICES_SIZE
#define IS_TRUE 1
#define IS_FALSE 0
#define STRL(s) s, strlen(s) + 1

typedef struct _vertoceNode
{
	unsigned short		isObstacle;
	unsigned short		visited;
	size_t				row_index;
	size_t				col_index;
	size_t				distance;
	_vertoceNode		*pervNode;
	char				name[0];
} VertoceNode;

static VertoceNode* graph[ROW_SIZE][COL_SIZE];

VertoceNode *createAndInitNode(size_t nameSize)
{
	VertoceNode *node = (VertoceNode *)malloc(sizeof(VertoceNode) + nameSize);
	if (node != NULL) {
		node->isObstacle = IS_FALSE;
		node->visited = IS_FALSE;
		node->row_index = 0;
		node->col_index = 0;
		node->distance = UINT_MAX;
		node->pervNode = NULL;
		memcpy(node->name, STRL("-"));
	}
	return node;
}

void initGraph()
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			VertoceNode *node = createAndInitNode(3);
			if (node == NULL) {
				printf("initGraph:call function[createAndInitNode(3)] error!\n");
				exit(-1);
			}
			node->row_index = i;
			node->col_index = j;
			graph[i][j] = node;
		}
	}
}

void destryGraph()
{
	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			delete graph[i][j];
			graph[i][j] = NULL;
		}
	}
}

void placeObstaclesToGraph()
{

	graph[0][2]->isObstacle = IS_TRUE;
	graph[0][3]->isObstacle = IS_TRUE;
	graph[0][4]->isObstacle = IS_TRUE;
	graph[0][5]->isObstacle = IS_TRUE;
	graph[0][6]->isObstacle = IS_TRUE;
	graph[0][7]->isObstacle = IS_TRUE;

	graph[1][2]->isObstacle = IS_TRUE;
	graph[2][2]->isObstacle = IS_TRUE;
	graph[2][3]->isObstacle = IS_TRUE;
	graph[2][4]->isObstacle = IS_TRUE;
	graph[2][5]->isObstacle = IS_TRUE;
	graph[2][6]->isObstacle = IS_TRUE;
	graph[2][7]->isObstacle = IS_TRUE;
	graph[1][7]->isObstacle = IS_TRUE;
	graph[3][4]->isObstacle = IS_TRUE;
	graph[3][5]->isObstacle = IS_TRUE;
}

vector<VertoceNode *> *getVerticeNeighborList(VertoceNode *node)
{
	vector<VertoceNode *> *neighborList = new vector<VertoceNode *>();

	//上方的的点
	if (node->row_index > 0) {
		VertoceNode *topNode = graph[node->row_index - 1][node->col_index];
		if (IS_TRUE != topNode->isObstacle) {
			neighborList->push_back(topNode);
		}
	}

	//下方的点
	if (node->row_index < ROW_SIZE - 1) {
		VertoceNode *botNode = graph[node->row_index + 1][node->col_index];
		if (IS_TRUE != botNode->isObstacle) {
			neighborList->push_back(botNode);
		}
	}

	//左边的点
	if (node->col_index > 0) {
		VertoceNode *leftNode = graph[node->row_index][node->col_index - 1];
		if (IS_TRUE != leftNode->isObstacle) {
			neighborList->push_back(leftNode);
		}
	}

	//右边的点
	if (node->col_index < COL_SIZE - 1) {
		VertoceNode *rightNode = graph[node->row_index][node->col_index + 1];
		if (IS_TRUE != rightNode->isObstacle) {
			neighborList->push_back(rightNode);
		}
	}

	return neighborList;

}

size_t length(VertoceNode *v, VertoceNode *u)
{
	//计算节点v到临近节点u的距离，因为生成的是网格图
	//所以每个点到每个点的距离都是1
	return v->distance + 1;
}

vector<VertoceNode *> *findShortestPath(VertoceNode *srcNode, VertoceNode *dstNode)
{
	queue<VertoceNode *> q;

	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		for (size_t j = 0; j < COL_SIZE; j++)
		{
			VertoceNode *node = graph[i][j];
			if (i == srcNode->row_index && j == srcNode->col_index) {
				node->distance = 0;
				node->pervNode = NULL;
			}

			q.push(node);
		}
	}

	while (!q.empty())
	{
		VertoceNode *v = q.front();
		q.pop();

		if (v->row_index == 3 && v->col_index == 6) {
			int hit = 1;
		}

		v->visited = IS_TRUE;

		//障碍物和未初始化的节点(因为存在障碍物，所以存在未初始化的节点)不参与寻路计算
		if (IS_TRUE == v->isObstacle || v->distance == UINT_MAX) {
			continue;
		}

		vector<VertoceNode *> *u = getVerticeNeighborList(v);
		for (auto uNode = u->begin(); uNode != u->end(); uNode++) {

			size_t alt = length(v, (*uNode));
			if (alt < (*uNode)->distance) {
				//找到最短距离
				(*uNode)->distance = alt;
				//记录最短距离的上一个节点
				(*uNode)->pervNode = v;
			}

		}

		delete u;
		u = NULL;

	}

	//将到目标点的最短路径添加到一个容器中返回
	vector<VertoceNode *> *shortestPathList = new vector<VertoceNode *>();
	VertoceNode *iterNode = dstNode;
	while (iterNode != NULL)
	{
		shortestPathList->push_back(iterNode);
		iterNode = iterNode->pervNode;
	}

	reverse(shortestPathList->begin(), shortestPathList->end());

	return shortestPathList;

}

void printGraph()
{
	printf("\t");
	for (size_t i = 0; i < COL_SIZE; i++)
	{
		printf("\t%d", i);
	}
	printf("\n");

	for (size_t i = 0; i < ROW_SIZE; i++)
	{
		printf("\t%d", i);

		for (size_t j = 0; j < COL_SIZE; j++)
		{
			VertoceNode *node = graph[i][j];
			if (IS_TRUE == node->isObstacle) {
				printf("\t%s", "*");
			}
			else {
				printf("\t%s", node->name);
			}
		}

		printf("\n");
	}

	printf("\n");
}

void printfShortestPathList(vector<VertoceNode *> *shortestPathList)
{
	size_t len = shortestPathList->size();

	for (size_t i = 0; i < len; i++)
	{
		VertoceNode *curNode = shortestPathList->at(i);
		VertoceNode *nextNode = NULL;
		if (i + 1 != len) {
			nextNode = shortestPathList->at(i + 1);
		}

		if (NULL != nextNode) {

			if (curNode->col_index != nextNode->col_index) {

				if (curNode->col_index < nextNode->col_index) {
					memcpy(curNode->name, STRL("→"));
				} else {
					memcpy(curNode->name, STRL("←"));
				}

			}

			if (curNode->row_index != nextNode->row_index) {

				if (curNode->row_index < nextNode->row_index) {
					memcpy(curNode->name, STRL("↓"));
				} else {
					memcpy(curNode->name, STRL("↑"));
				}

			}

		}

	}

	if (shortestPathList->size() >= 2) {
		VertoceNode *srcNode = shortestPathList->at(0);
		VertoceNode *dstNode = shortestPathList->at(shortestPathList->size() - 1);
		memcpy(srcNode->name, STRL("S"));
		memcpy(dstNode->name, STRL("D"));
	}

	printGraph();

}

int main()
{
	initGraph();
	placeObstaclesToGraph();
	printGraph();

	VertoceNode *srcNode = graph[0][0];
	VertoceNode *dstNode = graph[7][7];
	vector<VertoceNode *> *shortestPathList = findShortestPath(srcNode, dstNode);
	printfShortestPathList(shortestPathList);
	delete shortestPathList;
	shortestPathList = NULL;
	getchar();
	destryGraph();
	return 0;
}
