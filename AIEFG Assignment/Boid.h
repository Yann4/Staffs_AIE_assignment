#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include "Collision.h"
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

class Boid
{
private:
	position pos;

	position velocity;
	float maxVelocity = 2.0f;

	float rotation;
	float red, green, blue;
	float width, height;

	position wanderTarget;

	float wanderWeight = 1.0f;
	float seekWeight = 1.0f;

public:
	Boid();
	Boid(position pos);
	Boid& operator=(const Boid&) = default;

	void Update(float delta);
	void Render();

	void resolveCollision(position moveBy);

	//Steering
	position Wander();

	position aggregateSteering(position& wander);

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(pos.x, pos.z, width, height); }
};