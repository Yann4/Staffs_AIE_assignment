#pragma once

#include "Boid.h"
#include "State.h"
#include "ObjectTypes.h"
#include "MathHelper.h"

class Boid;

class HuntState : public State
{
private:
	Boid* boid;
	int hunger;
	BoidInfo* target;

public:
	HuntState(Boid* boid, State* sisterState);
	~HuntState();

	void giveSister(State* sister) { this->sister = sister; }

	void Enter();
	void Update(float delta, const std::vector<BoidInfo>& others);
	void Exit();

	bool shouldExit();

	void AccidentallyAte() { hunger--; }
private:
	void selectTarget(const std::vector<BoidInfo>& others);
	bool needNewTarget(const std::vector<BoidInfo>& others);
};
