#pragma once

#include <vector>

struct BoidInfo;

class State
{
protected:
	State* sister;

public:
	State() { sister = nullptr; }
	virtual void Enter() = 0;
	virtual void Update(float delta, const std::vector<BoidInfo>& others) = 0;
	virtual void Exit() = 0;

	virtual bool shouldExit() = 0;
};