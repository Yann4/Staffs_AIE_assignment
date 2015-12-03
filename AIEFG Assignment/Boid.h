#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include <random>

class Boid
{
private:
	SHAPE boidShape;
	position pos;
	position velocity;

	float rotation;
	float red, green, blue;

	position wanderTarget;
	float maxVelocity = 2.0f;

public:
	Boid();
	Boid(position pos, float rotation);

	void Update(float delta);
	void Render();

	position Wander();
};