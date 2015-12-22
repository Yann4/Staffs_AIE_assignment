#include "Graph.h"

Graph::Graph()
{
	root = nullptr;
	rootLocation = position(0, 0);
}

Graph::Graph(position rootLocation, float size, Obstacle** obstacles, int numObstacles) : rootLocation(rootLocation)
{
	root = nullptr;
	generateGraph(size, obstacles, numObstacles);
}

Graph::~Graph()
{
	GraphNode* temp = root;
	while (temp->down != nullptr)
	{
		GraphNode* row = temp->right;

		while (row->right != nullptr)
		{
			row = row->right;
			delete row->left;
		}
		delete row;
		temp = temp->down;
		delete temp->up;
	}
	delete temp;
}

void Graph::generateGraph(float size, Obstacle ** obstacles, int numObstacles)
{
	int width = 21;
	int height = 21;

	GraphNode* temp = root;
	temp = new GraphNode(position(rootLocation.x, rootLocation.z), OPEN, nullptr, nullptr, nullptr, nullptr);
	root = temp;
	
	for (int z = 1; z < height; z++)
	{
		temp->down = new GraphNode(position(rootLocation.x, rootLocation.z + z), OPEN, temp, nullptr, nullptr, nullptr);
		temp = temp->down;
	}

	GraphNode* trail;
	for (int z = 0; z < height; z++)
	{
		temp = root;

		for (int i = 0; i < z; i++)
		{
			temp = temp->down;
		}
		
		trail = temp->up;
		if (trail != nullptr)
		{
			trail = trail->right;
		}

		for (int x = 1; x < width; x++)
		{
			temp->right = new GraphNode(position(rootLocation.x + x, rootLocation.z + z), OPEN, trail, nullptr, temp, nullptr);
			temp = temp->right;

			if (trail != nullptr)
			{
				trail->down = temp;
				trail = trail->right;
			}
		}
	}

	for (int i = 0; i < numObstacles; i++)
	{
		position pos = obstacles[i]->GetPosition();
		pos.x -= rootLocation.x;
		pos.z -= rootLocation.z;

		temp = root;
		for (int x = 0; x < pos.x; x++)
		{
			temp = temp->right;
		}

		for (int y = 0; y < pos.z; y++)
		{
			temp = temp->down;
		}
		temp->type = OBSTACLE;
		temp->setColour(1, 0, 0);
	}
}

void Graph::RenderGraph()
{
	GraphNode* temp = root;
	int depth = 0;
	
	while (temp != nullptr)
	{
		while (temp != nullptr)
		{
			glPushMatrix();
				glTranslatef(temp->pos.x, 0.5f, temp->pos.z);
				glColor3f(temp->r, temp->g, temp->b);
				glutSolidSphere(0.1, 20, 20);
			glPopMatrix();

			glColor3f(1, 1, 1);
			if (temp->left != nullptr)
			{
				glBegin(GL_LINES);
					glVertex3f(temp->pos.x, 1.0f, temp->pos.z);
					glVertex3f(temp->left->pos.x, 1.0f, temp->left->pos.z);
				glEnd();
			}

			if (temp->right != nullptr)
			{
				glBegin(GL_LINES);
					glVertex3f(temp->pos.x, 1.0f, temp->pos.z);
					glVertex3f(temp->right->pos.x, 1.0f, temp->right->pos.z);
				glEnd();
			}

			if (temp->up != nullptr)
			{
				glBegin(GL_LINES);
					glVertex3f(temp->pos.x, 1.0f, temp->pos.z);
					glVertex3f(temp->up->pos.x, 1.0f, temp->up->pos.z);
				glEnd();
			}

			if (temp->down != nullptr)
			{
				glBegin(GL_LINES);
					glVertex3f(temp->pos.x, 1.0f, temp->pos.z);
					glVertex3f(temp->down->pos.x, 1.0f, temp->down->pos.z);
				glEnd();
			}
			temp = temp->right;
		}

		temp = root;
		for (int i = 0; i < depth; i++)
		{
			temp = temp->down;
		}
		depth++;
	}
}

GraphNode* Graph::getNearestNode(position p)
{
	position nodeLoc = position(round(p.x * 2) / 2, round(p.z * 2) / 2); //Round to the nearest .5 (Centre of each node)
	nodeLoc.x -= rootLocation.x;
	nodeLoc.z -= rootLocation.z;

	GraphNode* temp = root;
	for (int x = 0; x < nodeLoc.x; x++)
	{
		if (temp->right != nullptr)
		{
			temp = temp->right;
		}
	}

	for (int y = 0; y < nodeLoc.z; y++)
	{
		if (temp->down != nullptr)
		{
			temp = temp->down;
		}
	}
	return temp;
}

//Uses A*, and constant cost between each node
std::queue<GraphNode*> Graph::getPath(position a, position b, bool useInfluence)
{
	GraphNode* start = getNearestNode(a);
	GraphNode* end = getNearestNode(b);
	if (start == nullptr || end == nullptr)
	{
		return std::queue<GraphNode*>();
	}

	start->g_score = 0;
	start->h_score = heuristic(start, end);

	GraphNode* current = start;

	std::vector<GraphNode*> open;
	std::vector<GraphNode*> closed;

	open.push_back(current);
	
	while (!open.empty())
	{
		auto iterator = open.begin();
		float lowestF = 999999999;
		for (auto iter = open.begin(); iter != open.end(); iter++)
		{
			if ((*iter)->getF() < lowestF)
			{
				current = *iter;
				iterator = iter;
				lowestF = current->getF();
			}
		}

		if (current->pos.x == end->pos.x && current->pos.z == end->pos.z)
		{
			break;
		}

		open.erase(iterator);
		closed.push_back(current);

		auto neighbours = current->children();
		for (auto n : neighbours)
		{
			if (n == nullptr || n->type == OBSTACLE)
			{
				continue;
			}

			//If in closed set; continue
			bool inClosed = false;
			for (auto c : closed)
			{
				if (c->pos.x == n->pos.x && c->pos.z == n->pos.z)
				{
					inClosed = true;
					break;
				}
			}
			if (inClosed)
			{
				continue;
			}

			//distance between is 1
			float tent_g = current->g_score + 1;

			//If not in the open set (new node)
			bool inOpen = false;
			int index = 0;
			for (int o = 0; o < open.size(); o++)
			{
				if (open.at(o)->pos.x == n->pos.x && open.at(o)->pos.z == n->pos.z)
				{
					inOpen = true;
					index = o;
					break;
				}
			}
			if (!inOpen)
			{
				n->parent = current;
				n->g_score = tent_g;
				n->h_score = heuristic(n, end);
				if (useInfluence)
				{
					n->g_score += n->influence * 2;
				}
				open.push_back(n);
				continue;
			}
			else if (tent_g >= open.at(index)->g_score)
			{
				continue;
			}

			open.at(index)->parent = current;
			open.at(index)->g_score = tent_g;
			open.at(index)->h_score = heuristic(open.at(index), end);
			if (useInfluence)
			{
				n->h_score += n->influence * 20;
			}
		}
	}

	std::queue<GraphNode*> path = std::queue<GraphNode*>();

	GraphNode* temp = end;
	while (temp->parent != nullptr)
	{
		path.push(temp);
		temp = temp->parent;
	}

	for (auto n : open)
	{
		n->resetNode();
	}

	for (auto n : closed)
	{
		n->resetNode();
	}

	return path;
}

float Graph::heuristic(GraphNode * a, GraphNode * b)
{
	//Euclidian squared to eliminate repeated sqrt
	return (b->pos.x - a->pos.x) * (b->pos.x - a->pos.x) + (b->pos.z - a->pos.z) * (b->pos.z - a->pos.z);
}

void Graph::resetGraph()
{
	GraphNode* temp = root;
	int depth = 0;

	while (temp != nullptr)
	{
		while (temp != nullptr)
		{
			temp->resetNode();
			temp = temp->right;
		}

		temp = root;
		for (int i = 0; i < depth; i++)
		{
			temp = temp->down;
		}
		depth++;
	}
}
