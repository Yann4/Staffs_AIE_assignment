#pragma once
#include "Obstacle.h"
#include "ObjectTypes.h"
#include <vector>
#include <queue>
#include <list>

enum NodeType
{
	OPEN, OBSTACLE
};

struct GraphNode
{
	NodeType type;
	position pos;

	GraphNode* up;
	GraphNode* down;
	GraphNode* left;
	GraphNode* right;

	GraphNode* parent;

	float g_score;
	float h_score;

	float r, g, b;
	GraphNode() :pos(position()), up(nullptr), down(nullptr), left(nullptr), right(nullptr), type(OPEN)
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		parent = nullptr;
	}

	GraphNode(position pos, NodeType type, GraphNode* up, GraphNode* down, GraphNode* left, GraphNode* right) :pos(pos), up(up), down(down), left(left), right(right), type(type)
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		parent = nullptr;
	}

	std::vector<GraphNode*> children()
	{
		std::vector<GraphNode*> c;
		c.push_back(up);
		c.push_back(down);
		c.push_back(left);
		c.push_back(right);
		return c;
	}

	void setColour(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void resetNode()
	{
		parent = nullptr;
		g_score = 0;
		h_score = 0;
	}

	float getF()
	{
		return g_score + h_score;
	}
};

class Graph
{
private:
	GraphNode* root;
	position rootLocation;
public:
	Graph();
	Graph(position rootLocation, float size, Obstacle** obstacles, int numObstacles);
	~Graph();

	void generateGraph(float size, Obstacle** obstacles, int numObstacles);

	void RenderGraph();

	GraphNode* getNearestNode(position p);

	std::queue<GraphNode*> getPath(position a, position b);
	float heuristic(GraphNode* a, GraphNode* b);
};
