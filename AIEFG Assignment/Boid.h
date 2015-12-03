#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include <vector>
#include <random>

struct BoidInformation
{
	position pos;
	float rotation;
	position velocity;

	BoidInformation()
	{
		pos.x = 0;
		pos.z = 0;

		rotation = 0;

		velocity.x = 0;
		velocity.z = 0;
	}

	BoidInformation(position pos, position velocity, float rotation) : pos(pos), velocity(velocity), rotation(rotation)
	{}
};

struct Force
{
	float x;
	float y;
	Force() :x(0), y(0)
	{}
	Force(float x, float y) :x(x), y(y)
	{}
};

class Boid
{
private:
	SHAPE boidShape;
	position pos;
	float rotation;
	float red, green, blue;
	position vel;

	position wanderTarget;
	Force wander();
public:
	Boid();
	Boid(position pos, float rotation);

	void Update(int deltaTime);
	void Render();

	inline BoidInformation getData() { return BoidInformation(pos, vel, rotation); };
};