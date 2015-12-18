#pragma once

#include "Boid.h"
#include "State.h"
#include "ObjectTypes.h"
#include "MathHelper.h"

class Boid;

class BecomeObstacleState : public State
{
private:
	Boid* boid;
	position location;

public:
	BecomeObstacleState(Boid* boid, State* sisterState);

	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();
};