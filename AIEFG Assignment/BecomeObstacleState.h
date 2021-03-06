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
	Graph* graph;
	position location;

public:
	BecomeObstacleState(Boid* boid, Graph* graph);

	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();
};