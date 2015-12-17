#pragma once
#include "ObjectTypes.h"
#include <stdlib.h>
#include "gl\glut.h"
#include "Enums.h"
#include "Collision.h"
#include "MathHelper.h"
#include "Graph.h"

#include "State.h"
#include "EscapeState.h"
#include "FlockState.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <array>
#include <stack>

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
	friend class EscapeState;
	friend class FlockState;

private:
	int id;
	position pos;

	position velocity;
	float maxVelocity = 2.0f;

	float rotation;
	float red, green, blue;
	float width, height;

	position wanderTarget;

	float wanderWeight = 0.5f;

	float alignWeight = 0.5f;
	float coheseWeight = 0.3f;
	float separateWeight = 0.5f;

	float avoidWallWeight = 10.0f;

	std::vector<Wall>* walls;

	std::stack<position> path;

	State* currentState;

public:
	Boid();
	Boid(char id, position pos, Graph* g, std::vector<Wall>* walls);
	Boid& operator=(const Boid&) = default;

	void Update(float delta, const std::vector<BoidInfo>& others);
	void Render();
	void resolveCollision(position moveBy);

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(pos.x, pos.z, width, height); }
	BoidInfo getInfo() { return BoidInfo(id, pos, velocity); }

private:
	void giveGoodGuyFSM();
	void giveBadGuyFSM();

	void UpdateLocation(position steeringForce, float delta);

	std::vector<BoidInfo> getNeighbourhood(const std::vector<BoidInfo>& allBoids);
	
	void givePath(std::stack<position> path);

	//Steering
	position Seek(position seekTo);

	position Wander();

	//Flocking
	position Alignment(const std::vector<BoidInfo>& neighbours);
	position Cohesion(const std::vector<BoidInfo>& neighbours);
	position Separation(const std::vector<BoidInfo>& neighbours);

	position followPath();

	position WallAvoidance();

	position aggregateSteering(position& wander, position& separate, position& align, position& cohese, position& avoidWalls);
};