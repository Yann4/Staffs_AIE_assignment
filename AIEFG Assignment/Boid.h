#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include "Collision.h"
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
#include <array>

struct BoidInfo
{
	int id;
	position pos;
	position velocity;

	BoidInfo() :pos(position()), velocity(position()), id(-1)
	{}

	BoidInfo(int id, position pos, position vel) : id(id), pos(pos), velocity(vel)
	{}
};

struct Wall
{
	position start;
	position end;

	Wall() : start(position()), end(position())
	{}

	Wall(position start, position end) : start(start), end(end)
	{}
};

class Boid
{
private:
	int id;
	position pos;

	position velocity;
	float maxVelocity = 2.0f;

	float rotation;
	float red, green, blue;
	float width, height;

	position wanderTarget;

	float wanderWeight = 0.1f;

	float alignWeight = 0; //0.5f;
	float coheseWeight = 0;// 0.3f;
	float separateWeight = 0;// 0.5f;

	float avoidWallWeight = 1.0f;

	std::vector<Wall>* walls;

public:
	Boid();
	Boid(char id, position pos, std::vector<Wall>* walls);
	Boid& operator=(const Boid&) = default;

	void Update(float delta, const std::vector<BoidInfo>& others);
	void Render();

	void resolveCollision(position moveBy);

	std::vector<BoidInfo> getNeighbourhood(const std::vector<BoidInfo>& allBoids);

	//Steering
	position Seek(position seekTo);

	position Wander();

	//Flocking
	position Alignment(const std::vector<BoidInfo>& neighbours);
	position Cohesion(const std::vector<BoidInfo>& neighbours);
	position Separation(const std::vector<BoidInfo>& neighbours);

	position WallAvoidance();

	position aggregateSteering(position& wander, position& separate, position& align, position& cohese, position& avoidWalls);

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(pos.x, pos.z, width, height); }
	BoidInfo getInfo() { return BoidInfo(id, pos, velocity); }
};