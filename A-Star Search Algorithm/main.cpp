#include <stdio.h>
#include <stdlib.h>

#include <string>

using std::string;

class GraphNode {

public:

	GraphNode() :
		is_obstacle_(false),
		row_index_(0),
		col_index_(0),
		f(0),
		g(0),
		h(0),
		display_mark_("-"),
		pervNode(nullptr)
	{
	};

	bool   is_obstacle_;
	size_t row_index_;
	size_t col_index_;
	size_t f;
	size_t g;
	size_t h;
	string display_mark_;
	GraphNode *pervNode;
};

class Graph {

public:

	Graph() 
		:Graph(8, 8)
	{
	};

	Graph(size_t rows, size_t cols) :
		rows_(rows),
		cols_(cols)
	{
		graph_ = (GraphNode **)::operator new(rows * cols * sizeof(GraphNode *));
		for (size_t i = 0; i < rows_; i++)
		{
			for (size_t j = 0; j < cols_; j++)
			{
				this->PutNode(nullptr, i, j);
			}
		}
	};

	void Print()
	{
		printf("\t");
		for (size_t i = 0; i < cols_; i++)
		{
			printf("%d\t", i);
		}
		printf("\n");

		for (size_t i = 0; i < rows_; i++)
		{
			printf("%d\t", i);
			for (size_t j = 0; j < cols_; j++)
			{
				GraphNode *node = this->FindByIndex(i, j);
				if (nullptr != node) {
					
					printf("%s\t", node->display_mark_.c_str());

				} else {

					printf("%s\t", "N");

				}
			}
			printf("\n");
		}
		printf("\n");

	};

	void PutNode(GraphNode *graphNode)
	{
		if (nullptr == graphNode) {
			throw "graph::PutNode error[graphNode is null!]";
		}
		if (!CheckIndexIsValid(graphNode->row_index_, graphNode->col_index_)) {
			throw "graph::PutNode error[index out of bounds]";
		}

		this->SetLocation(graphNode, graphNode->row_index_, graphNode->col_index_);

	};

	void PutNode(GraphNode *graphNode, size_t row_index, size_t col_index)
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

	GraphNode *FindByIndex(size_t row_index, size_t col_index)
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

	bool CheckIndexIsValid(size_t row_index, size_t col_index)
	{
		if (row_index > rows_ || col_index > cols_) {
			return false;
		}
		else {
			return true;
		}
	}

private:
	size_t rows_;
	size_t cols_;
	GraphNode **graph_;

	void SetLocation(GraphNode *graphNode, size_t row_index, size_t col_index)
	{
		GraphNode **locationAddr = this->GetLoaction(row_index, col_index);
		*locationAddr = graphNode;
	}

	GraphNode **GetLoaction(size_t row_index, size_t col_index)
	{
		return (graph_ + (row_index * cols_) + col_index);
	}

};

int main() {

	Graph *graph = new Graph;
	GraphNode *node = new GraphNode;
	graph->PutNode(node);

	graph->Print();

	getchar();

	return 0;

}