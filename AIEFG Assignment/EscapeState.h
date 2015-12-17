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
	position exit1;
	position exit2;

public:
	EscapeState(Boid* boid, Graph* graph, State* sisterState);
	
	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();
};