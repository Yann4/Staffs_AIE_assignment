#include "FlockState.h"

FlockState::FlockState(Boid * boid, State * sisterState): boid(boid)
{
	sister = sisterState;
}

void FlockState::Enter()
{
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
}

void FlockState::Exit()
{
	if (sister != nullptr)
	{
		boid->currentState = sister;
		boid->currentState->Enter();
		delete this;
	}
}

bool FlockState::shouldExit()
{
	return false;
}
