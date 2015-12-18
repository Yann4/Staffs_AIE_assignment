#pragma once

#include "Boid.h"
#include "State.h"
#include "ObjectTypes.h"
#include "MathHelper.h"
#include "Graph.h"
#include <queue>
#include <vector>

class Boid;

class PatrolState : public State
{
private:
	Boid* boid;
	Graph* graph;
	position start;
	position end;
	position currentGoal;

public:
	PatrolState(Boid* boid, State* sisterState, Graph * graph, position start, position end);

	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();
};