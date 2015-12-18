#include "PatrolState.h"

PatrolState::PatrolState(Boid * boid, State * sisterState, Graph * graph, position start, position end): boid(boid), graph(graph), start(start), end(end)
{
	sister = sisterState;
	currentGoal = start;
	threshold = randomInRange(1, 10);
}

void PatrolState::Enter()
{
	auto pathQ = graph->getPath(boid->getInfo().pos, currentGoal);
	std::stack<position> path;

	while (!pathQ.empty())
	{
		path.push(pathQ.front()->pos);
		pathQ.pop();
	}
	boid->givePath(path);
}

void PatrolState::Update(float delta, const std::vector<BoidInfo>& others)
{
	float radius = 0.5;
	radius *= radius;

	if (distSquared(boid->pos, currentGoal) < radius)
	{
		if (currentGoal.x == start.x && currentGoal.z == start.z)
		{
			currentGoal = end;
		}
		else
		{
			currentGoal = start;
		}

		auto pathQ = graph->getPath(boid->getInfo().pos, currentGoal);
		std::stack<position> path;

		while (!pathQ.empty())
		{
			path.push(pathQ.front()->pos);
			pathQ.pop();
		}
		boid->givePath(path);
	}

	position steeringForce = boid->followPath();
	boid->UpdateLocation(steeringForce, delta);

	int targets = 0;
	for (BoidInfo i : others)
	{
		if (i.target)
		{
			targets++;
		}
	}

	if (shouldExit(targets))
	{
		Exit();
	}
}

void PatrolState::Exit()
{
	if (sister != nullptr)
	{
		boid->currentState = sister;
		boid->currentState->Enter();
		delete this;
	}
}

bool PatrolState::shouldExit(int numTargets)
{
	if (numTargets >= threshold)
	{
		return true;
	}
	return false;
}
