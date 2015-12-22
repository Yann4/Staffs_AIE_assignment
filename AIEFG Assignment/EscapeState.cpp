#include "EscapeState.h"

EscapeState::EscapeState(Boid* boid, Graph* graph, State* sisterState, bool useInfluence): boid(boid), graph(graph), useInfluence(useInfluence)
{
	exitLoc = position(10.5f, 11.5f);
	sister = sisterState;
}

void EscapeState::Enter()
{
	std::stack<position> path;
	auto pathQ = graph->getPath(boid->getInfo().pos, exitLoc, useInfluence);
	
	while (!pathQ.empty())
	{
		path.push(pathQ.front()->pos);
		pathQ.pop();
	}
	numNodes = path.size();
	boid->givePath(path);
}

void EscapeState::Update(float delta, const std::vector<BoidInfo>& others)
{
	position pathFollow = boid->followPath();
	boid->UpdateLocation(pathFollow, delta);

	if (useInfluence)
	{
		std::stack<position> path;
		auto pathQ = graph->getPath(boid->getInfo().pos, exitLoc, useInfluence);

		while (!pathQ.empty())
		{
			path.push(pathQ.front()->pos);
			pathQ.pop();
		}
		boid->givePath(path);
		numNodes = path.size();
	}

	if (shouldExit())
	{
		Exit();
	}
}

void EscapeState::Exit()
{
	if (sister != nullptr)
	{
		boid->currentState = sister;
		boid->currentState->Enter();
	}
}

bool EscapeState::shouldExit()
{
	float radius = 0.5f;
	radius *= radius;

	position boidPos = boid->getInfo().pos;

	float distSq = distSquared(boidPos, exitLoc);

	if (distSq < radius)
	{
		return true;
	}

	return false;
}

