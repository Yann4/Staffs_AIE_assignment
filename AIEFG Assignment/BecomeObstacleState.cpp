#include "BecomeObstacleState.h"

BecomeObstacleState::BecomeObstacleState(Boid * boid, State * sisterState): boid(boid)
{
	sister = sisterState;
}

void BecomeObstacleState::Enter()
{
	location = position(randomInRange(0, 21), randomInRange(0, 12));
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
	}
}

void BecomeObstacleState::Exit()
{
}

bool BecomeObstacleState::shouldExit()
{
	return false;
}
