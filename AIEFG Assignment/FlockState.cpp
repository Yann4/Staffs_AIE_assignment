#include "FlockState.h"

FlockState::FlockState(Boid * boid, State * sisterState): boid(boid)
{
	sister = sisterState;
}

void FlockState::Enter()
{
	lifeTime = 0;
	boid->isTarget = true;
}

void FlockState::Update(float delta, const std::vector<BoidInfo>& others)
{
	std::vector<BoidInfo> neighbours = boid->getNeighbourhood(others);
	position wander = boid->Wander();

	position separate = boid->Separation(neighbours);

	position alignment = boid->Alignment(neighbours);

	position cohese = boid->Cohesion(neighbours);

	position avoidWalls = boid->WallAvoidance();

	position steeringForce = boid->aggregateSteering(wander, separate, alignment, cohese, avoidWalls);

	boid->UpdateLocation(steeringForce, delta);

	if (shouldExit())
	{
		Exit();
	}
}

void FlockState::Exit()
{
	if (sister != nullptr)
	{
		boid->currentState = sister;
		boid->currentState->Enter();
	}
}

bool FlockState::shouldExit()
{
	if (lifeTime < retirementAge)
	{
		lifeTime++;
		return false;
	}
	else
	{
		return true;
	}
}
