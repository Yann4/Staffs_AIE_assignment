#pragma once

#include "Boid.h"
#include "State.h"
#include "ObjectTypes.h"
#include "MathHelper.h"

class Boid;

class FlockState : public State
{
private:
	Boid* boid;

public:
	FlockState(Boid* boid, State* sisterState);

	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();
};
