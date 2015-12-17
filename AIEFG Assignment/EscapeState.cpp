#include "EscapeState.h"

EscapeState::EscapeState(Boid* boid, Graph* graph, State* sisterState): boid(boid), graph(graph)
{
	//Add sister state
	exit1 = position(2.5f, 12.5f);
	exit2 = position(17.5f, 12.5f);
	sister = sisterState;
}

void EscapeState::Enter()
{
	position exitLoc;
	float r = randomInRange(-1, 1);
	if (r < 0)
	{
		exitLoc = exit1;
	}
	else
	{
		exitLoc = exit2;
	}

	std::stack<position> path;
	auto pathQ = graph->getPath(boid->getInfo().pos, exitLoc);
	
	while (!pathQ.empty())
	{
		path.push(pathQ.front()->pos);
		pathQ.pop();
	}

	boid->givePath(path);
}

void EscapeState::Update(float delta, const std::vector<BoidInfo>& others)
{
	position pathFollow = boid->followPath();
	boid->UpdateLocation(pathFollow, delta);

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
		sister = nullptr;
		delete this;
	}
}

bool EscapeState::shouldExit()
{
	float radius = 0.5f;
	radius *= radius;

	position boidPos = boid->getInfo().pos;

	float distSq = ((boidPos.x - exit1.x) * (boidPos.x - exit1.x)) + ((boidPos.z - exit1.z) * (boidPos.z - exit1.z));
	distSq = abs(distSq);

	if (distSq < radius)
	{
		return true;
	}

	distSq = ((boidPos.x - exit2.x) * (boidPos.x - exit2.x)) + ((boidPos.z - exit2.z) * (boidPos.z - exit2.z));
	distSq = abs(distSq);

	if (distSq < radius)
	{
		return true;
	}

	return false;
}

