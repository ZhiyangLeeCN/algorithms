#include <iostream>

#include <vector>
#include <set>
#include <stack>
#include <string>

using std::vector;
using std::set;
using std::stack;
using std::string;

class GraphNode {

public:

	GraphNode() :
		is_obstacle_(false),
		is_closed_(false),
		row_index_(0),
		col_index_(0),
		f(UINT_MAX),
		g(UINT_MAX),
		h(UINT_MAX),
		display_mark_("-"),
		pervNode(nullptr)
	{
	}

	bool   is_obstacle_;
	bool   is_closed_;
	int    row_index_;
	int    col_index_;
	double f;
	double g;
	double h;
	string display_mark_;
	GraphNode *pervNode;
};

class Graph {

public:

	Graph() 
		:Graph(8, 8)
	{
	};

	Graph(int rows, int cols) :
		rows_(rows),
		cols_(cols)
	{
		graph_ = (GraphNode **)::operator new(rows * cols * sizeof(GraphNode *));
		for (int i = 0; i < rows_; i++)
		{
			for (int j = 0; j < cols_; j++)
			{
				this->PutNode(nullptr, i, j);
			}
		}
	}

	~Graph()
	{
		for (int i = 0; i < rows_; i++)
		{
			for (int j = 0; j < cols_; j++)
			{
				GraphNode *node = this->FindByIndex(i, j);
				if (nullptr != node) {
					delete node;
				}
			}
		}

		delete graph_;
	}

	void Print()
	{
		//std::cout << "\t";
		//for (int i = 0; i < cols_; i++)
		//{
		//	std::cout << i << "\t";
		//}
		//std::cout << "\n";

		for (int i = 0; i < rows_; i++)
		{
			//std::cout << i << "\t";
			for (int j = 0; j < cols_; j++)
			{
				GraphNode *node = this->FindByIndex(i, j);
				if (nullptr != node) {
					
					if (node->is_obstacle_) {

						std::cout << "W";

					} else {

						std::cout << node->display_mark_;

					}

				} else {

					std::cout << "N";

				}
			}
			std::cout << "\n";
		}
		std::cout << "\n";

	}

	void PutNode(GraphNode *graphNode)
	{
		if (nullptr == graphNode) {
			throw "graph::PutNode error[graphNode is null!]";
		}
		if (!CheckIndexIsValid(graphNode->row_index_, graphNode->col_index_)) {
			throw "graph::PutNode error[index out of bounds]";
		}

		this->SetLocation(graphNode, graphNode->row_index_, graphNode->col_index_);

	}

	void PutNode(GraphNode *graphNode, int row_index, int col_index)
	{
		if (!CheckIndexIsValid(row_index, col_index)) {
			throw "graph::PutNode error[index out of bounds]";
		}

		if (nullptr != graphNode) {
			graphNode->row_index_ = row_index;
			graphNode->col_index_ = col_index;
		}

		this->SetLocation(graphNode, row_index, col_index);

	}

	double estimatedDistance(GraphNode *currentNode, GraphNode *dstNode)
	{
		double cur_row_index = currentNode->row_index_;
		double cur_col_index = currentNode->col_index_;

		double dst_row_index = dstNode->row_index_;
		double dst_col_index = dstNode->col_index_;

		return std::abs(cur_row_index - dst_row_index) + std::abs(cur_col_index - dst_col_index);
	}

	//A* serach
	vector<GraphNode *> *FindPath(GraphNode *srcNode, GraphNode *dstNode)
	{

		bool reached = false;
		set<GraphNode *> openList;
		
		srcNode->f = 0;
		srcNode->g = 0;
		srcNode->h = 0;
		openList.insert(srcNode);

		while (!openList.empty())
		{
			GraphNode *currentNode = *openList.begin();
			openList.erase(openList.begin());

			currentNode->is_closed_ = true;

			//find target
			if (currentNode->col_index_ == dstNode->col_index_ && 
				currentNode->row_index_ == dstNode->row_index_) {
				reached = true;
				break;
			}

			if (currentNode->row_index_ == 0 && currentNode->col_index_ == 5) {
				int hit = 0;
			}

			vector<GraphNode *> *neighbors = this->GetNeighborsByNode(currentNode);
			if (!neighbors->empty()) {

				for (auto iter = neighbors->begin(); iter != neighbors->end(); iter++)
				{
					GraphNode *nNode = *iter;
					double nG = currentNode->g + 1;
					double nH = this->estimatedDistance(nNode, dstNode);
					double nF = nG + nH;

					if (nNode->f > nF) {

						nNode->f = nF;
						nNode->g = nG;
						nNode->h = nH;
						nNode->pervNode = currentNode;

						openList.insert(nNode);

					}
				}

			}

			delete neighbors;
		}

		vector<GraphNode *> *pathList = new vector<GraphNode *>;
		if (reached) {

			stack<GraphNode *> pathStack;
			GraphNode *iterNode = dstNode;
			while (nullptr != iterNode)
			{
				pathStack.push(iterNode);
				iterNode = iterNode->pervNode;
			}

			while (!pathStack.empty())
			{
				pathList->push_back(pathStack.top());
				pathStack.pop();
			}

		}

		return pathList;

	}

	vector<GraphNode *> *GetNeighborsByNode(GraphNode *node)
	{
		vector<GraphNode *> filter;
		vector<GraphNode *> *result = new vector<GraphNode *>;

		//��
		if (node->col_index_ > 0) {
			GraphNode *leftNode = this->FindByIndex(node->row_index_, node->col_index_ - 1);
			filter.push_back(leftNode);
		}

		//��
		if (node->col_index_ < cols_ - 1) {
			GraphNode *rightNode = this->FindByIndex(node->row_index_, node->col_index_ + 1);
			filter.push_back(rightNode);
		}

		//��
		if (node->row_index_ > 0) {
			GraphNode *topNode = this->FindByIndex(node->row_index_ - 1, node->col_index_);
			filter.push_back(topNode);
		}

		//��
		if (node->row_index_ < rows_ - 1) {
			GraphNode *bottomNode = this->FindByIndex(node->row_index_ + 1, node->col_index_);
			filter.push_back(bottomNode);
		}

		for (auto iter = filter.begin(); iter != filter.end(); iter++)
		{
			GraphNode *iterNode = *iter;
			if (nullptr != iterNode && 
				!iterNode->is_closed_ && 
				!iterNode->is_obstacle_) {
				result->push_back(iterNode);
			}
		}

		return result;
	}

	GraphNode *FindByIndex(int row_index, int col_index)
	{
		if (!CheckIndexIsValid(row_index, col_index)) {
			return nullptr;
		}
		else
		{
			GraphNode **location = this->GetLoaction(row_index, col_index);
			return *location;
		}
	}

	bool CheckIndexIsValid(int row_index, int col_index)
	{
		if (row_index > rows_ || col_index > cols_) {
			return false;
		}
		else {
			return true;
		}
	}

	int GetRows()
	{
		return rows_;
	}

	int GetCols()
	{
		return cols_;
	}

private:
	int rows_;
	int cols_;
	GraphNode **graph_;

	void SetLocation(GraphNode *graphNode, int row_index, int col_index)
	{
		GraphNode **locationAddr = this->GetLoaction(row_index, col_index);
		*locationAddr = graphNode;
	}

	GraphNode **GetLoaction(int row_index, int col_index)
	{
		return (graph_ + (row_index * cols_) + col_index);
	}

};

int main() {

	Graph *graph = new Graph;

	for (int i = 0; i < graph->GetRows(); i++)
	{
		for (int j = 0; j < graph->GetCols(); j++)
		{
			graph->PutNode(new GraphNode, i, j);
		}
	}

	graph->FindByIndex(1, 2)->is_obstacle_ = true;
	graph->FindByIndex(1, 3)->is_obstacle_ = true;
	graph->FindByIndex(1, 4)->is_obstacle_ = true;
	graph->FindByIndex(2, 4)->is_obstacle_ = true;
	graph->FindByIndex(3, 4)->is_obstacle_ = true;
	graph->FindByIndex(4, 4)->is_obstacle_ = true;
	graph->FindByIndex(5, 4)->is_obstacle_ = true;
	graph->FindByIndex(5, 3)->is_obstacle_ = true;
	graph->FindByIndex(5, 2)->is_obstacle_ = true;

	graph->Print();

	GraphNode *src = graph->FindByIndex(0, 0);
	GraphNode *dst = graph->FindByIndex(7, 7);

	vector<GraphNode *> *pathList = graph->FindPath(src, dst);
	for (auto iter = pathList->begin(); iter != pathList->end(); iter++)
	{
		GraphNode *node = *iter;
		node->display_mark_ = "*";
	}

	graph->Print();

	for (auto iter = pathList->begin(); iter != pathList->end(); iter++)
	{
		GraphNode *node = *iter;
		std::cout << "(" << node->row_index_ << ", " << node->col_index_ << ", [f:" << node->f << ", g:" << node->g << ", h:" << node->h << "]" << ")";
	}

	int pasue;
	std::cin >> pasue;

	delete pathList;
	delete graph;

	return 0;

}