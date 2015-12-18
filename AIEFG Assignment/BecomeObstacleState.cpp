#include "BecomeObstacleState.h"

BecomeObstacleState::BecomeObstacleState(Boid * boid, State * sisterState): boid(boid)
{
}

void BecomeObstacleState::Enter()
{
}

void BecomeObstacleState::Update(float delta, const std::vector<BoidInfo>& others)
{
}

void BecomeObstacleState::Exit()
{
}

bool BecomeObstacleState::shouldExit()
{
	return false;
}
