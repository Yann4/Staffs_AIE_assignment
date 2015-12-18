#include "BecomeObstacleState.h"

BecomeObstacleState::BecomeObstacleState(Boid * boid, Graph* graph, State * sisterState) : boid(boid), graph(graph)
{
	sister = sisterState;
}

void BecomeObstacleState::Enter()
{
	location = position(randomInRange(0, 20), randomInRange(0, 11));
	location = position((int)location.x + 0.5, (int)location.z + 0.5);
	boid->isTarget = false;
}

void BecomeObstacleState::Update(float delta, const std::vector<BoidInfo>& others)
{
	float radius = 0.5;
	radius *= radius;

	if (distSquared(boid->pos, location) > radius)
	{
		position steer = boid->Seek(location);
		boid->UpdateLocation(steer, delta);
	}
	else
	{
		boid->pos = location;
		graph->getNearestNode(location)->type = OBSTACLE;
	}
}

void BecomeObstacleState::Exit()
{
}

bool BecomeObstacleState::shouldExit()
{
	return false;
}
