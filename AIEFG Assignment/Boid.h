#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include "Collision.h"
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

struct BoidInfo
{
	char id;
	position pos;
	position velocity;

	BoidInfo() :pos(position()), velocity(position()), id('#')
	{}

	BoidInfo(char id, position pos, position vel) : id(id), pos(pos), velocity(vel)
	{}
};

class Boid
{
private:
	char id;
	position pos;

	position velocity;
	float maxVelocity = 2.0f;

	float rotation;
	float red, green, blue;
	float width, height;

	position wanderTarget;

	float wanderWeight = 0.01f;

	float alignWeight = 1.0f;
	float coheseWeight = 1.0f;
	float separateWeight = 1.0f;

public:
	Boid();
	Boid(char id, position pos);
	Boid& operator=(const Boid&) = default;

	void Update(float delta, std::vector<BoidInfo> others);
	void Render();

	void resolveCollision(position moveBy);

	//Steering
	position Wander();

	position Alignment(const std::vector<BoidInfo>& neighbours);
	position Cohesion(const std::vector<BoidInfo>& neighbours);
	position Separation(const std::vector<BoidInfo>& neighbours);

	position aggregateSteering(position& wander, position& separate);

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(pos.x, pos.z, width, height); }
	BoidInfo getInfo() { return BoidInfo(id, pos, velocity); }
};