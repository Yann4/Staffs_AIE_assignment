#pragma once

#include "State.h"
#include "Boid.h"
#include "ObjectTypes.h"
#include "Graph.h"
#include "MathHelper.h"

class Boid;

class EscapeState : public State
{
private:
	Boid* boid;

	Graph* graph;
	position exitLoc;

	int numNodes;
	bool useInfluence;

public:
	EscapeState(Boid* boid, Graph* graph, State* sisterState, bool useInfluence = true);
	
	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();
};