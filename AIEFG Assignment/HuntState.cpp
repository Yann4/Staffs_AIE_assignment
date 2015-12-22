#include "HuntState.h"
#include "Boid.h"

HuntState::HuntState(Boid* boid, State* sisterState) : boid(boid)
{
	sister = sisterState;
	target = new BoidInfo(-99, position(), position(), false);
}

HuntState::~HuntState()
{
	delete target;
	boid = nullptr; 
}

void HuntState::Enter()
{
	hunger = (int)randomInRange(1, 3);
}

void HuntState::Update(float delta, const std::vector<BoidInfo>& others)
{
	if (needNewTarget(others))
	{
		hunger--;
		bool anyTargets = false;
		for (BoidInfo i : others)
		{
			if (i.target)
			{
				anyTargets = true;
				break;
			}
		}

		if (shouldExit() || !anyTargets)
		{
			Exit();
			return;
		}
		selectTarget(others);
	}
	else
	{
		for (BoidInfo i : others)
		{
			if (i.id == target->id)
			{
				target->pos = i.pos;
				break;
			}
		}
	}

	position steerForce = boid->Seek(target->pos);
	position wallAvoid = boid->WallAvoidance();
	steerForce.x += wallAvoid.x;
	steerForce.z += wallAvoid.z;
	boid->UpdateLocation(steerForce, delta);
	
}

bool HuntState::shouldExit()
{
	if (hunger <= 0)
	{
		return true;
	}
	return false;
}

void HuntState::Exit()
{
	if (sister != nullptr)
	{
		boid->currentState = sister;
		boid->currentState->Enter();
	}
}

bool HuntState::needNewTarget(const std::vector<BoidInfo>& others)
{
	for (BoidInfo i : others)
	{
		if (i.id == target->id)
		{
			return false;
		}
	}
	return true;
}

void HuntState::selectTarget(const std::vector<BoidInfo>& others)
{
	unsigned int ind = 0;

	while (ind + 1 < others.size() && !others.at(ind).target)
	{
		ind++;
	}

	target->id = others.at(ind).id;
	target->pos = others.at(ind).pos;
	target->velocity = others.at(ind).velocity;
	target->target = others.at(ind).target;
}