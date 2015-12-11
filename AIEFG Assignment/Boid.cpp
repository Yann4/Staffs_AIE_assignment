#include "Boid.h"
#include <iostream>

Boid::Boid()
{
	pos = position(0,0);
	wanderTarget = position(0, 0);

	rotation = 0;

	red = 0;
	green = 255;
	blue = 0;

	velocity = position(0, 0);
	wanderTarget = pos;
	width = 0.25;
	height = 0.5;
	walls = nullptr;
}

Boid::Boid(char id, position pos, std::vector<Wall>* walls) : pos(pos), id(id), walls(walls)
{
	red = 0;
	green = 255;
	blue = 0;

	rotation = 0;

	velocity = position(0, 0);
	wanderTarget = pos;

	width = 0.25;
	height = 0.5;
}

float randomInRange(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

position normalise(position p)
{
	if (p.x == 0 && p.z == 0)
	{
		return position(0, 0);
	}
	float mag = sqrtf(p.x * p.x + p.z * p.z);
	p.x /= mag;
	p.z /= mag;
	return p;
}

position truncate(position p, float maxMag)
{
	float mag = sqrtf(p.x * p.x + p.z * p.z);
	if (mag > maxMag)
	{
		p.x /= mag;
		p.z /= mag;

		p.x *= maxMag;
		p.z *= maxMag;
	}
	return p;
}

position Boid::Seek(position seekTo)
{
	position desiredVel = seekTo;
	desiredVel.x -= pos.x;
	desiredVel.z -= pos.z;
	
	desiredVel = normalise(desiredVel);

	desiredVel.x *= maxVelocity;
	desiredVel.z *= maxVelocity;

	desiredVel.x -= velocity.x;
	desiredVel.z -= velocity.z;

	return desiredVel;
}

position Boid::Wander()
{
	float wanderJitter = 2;
	float wanderDistance = 2;
	float wanderRadius = 10;

	wanderTarget.x += randomInRange(-1, 1) * wanderJitter;
	wanderTarget.z += randomInRange(-1, 1) * wanderJitter;

	wanderTarget = normalise(wanderTarget);

	wanderTarget.x *= wanderRadius;
	wanderTarget.z *= wanderRadius;

	position targetLocal = wanderTarget;
	targetLocal.x += wanderDistance;

	position targetWorld;

	targetWorld.x = pos.x + targetLocal.x;
	targetWorld.z = pos.z + targetLocal.z;

	targetWorld.x += cos(rotation) * (targetLocal.x) - sin(rotation) * (targetLocal.z);
	targetWorld.z += sin(rotation) * (targetLocal.x) - cos(rotation) * (targetLocal.z);


	position wanderForce = targetWorld;
	wanderForce.x -= pos.x;
	wanderForce.z -= pos.z;

	return wanderForce;
}

position Boid::Alignment(const std::vector<BoidInfo>& neighbours)
{
	position steeringForce = position();
	int count = 0;
	for (BoidInfo neighbour : neighbours)
	{
		if (neighbour.id == id)
		{
			continue;
		}

		steeringForce.x += neighbour.velocity.x;
		steeringForce.z += neighbour.velocity.z;
		count++;
	}

	if (count > 0)
	{
		steeringForce.x /= count;
		steeringForce.z /= count;

		steeringForce.x -= velocity.x;
		steeringForce.z -= velocity.z;
	}

	return steeringForce;
}

position Boid::Cohesion(const std::vector<BoidInfo>& neighbours)
{
	position steeringForce = position();
	position centreOfMass = position();
	int count = 0;

	for (BoidInfo neighbour : neighbours)
	{
		if (neighbour.id == id)
		{
			continue;
		}

		centreOfMass.x += neighbour.pos.x;
		centreOfMass.z += neighbour.pos.z;
		count++;
	}

	if (count > 0)
	{
		centreOfMass.x /= count;
		centreOfMass.z /= count;

		steeringForce = Seek(centreOfMass);
	}
	return steeringForce;
}

position Boid::Separation(const std::vector<BoidInfo>& neighbours)
{
	position steeringForce = position();

	for (BoidInfo neighbour : neighbours)
	{
		if (neighbour.id == id)
		{
			continue;
		}
		position toBoid = pos;
		toBoid.x -= neighbour.pos.x;
		toBoid.z -= neighbour.pos.z;

		float dist = sqrt(toBoid.x * toBoid.x + toBoid.z * toBoid.z);
		toBoid = normalise(toBoid);
		toBoid.x /= dist;
		toBoid.x /= dist;

		steeringForce.x += toBoid.x;
		steeringForce.z += toBoid.z;
	}

	return steeringForce;
}

bool lineIntersection(position a, position b, position c, position d, position& pointOfIntersection)
{
	position s1 = position(b.x - a.x, b.z - a.z);
	position s2 = position(d.x - c.x, d.z - c.z);

	float denominator = -s2.x * s1.z + s1.x * s2.z;

	//Lines are parallell
	if (denominator == 0)
	{
		return false;
	}

	float s = (-s1.z * (a.x - c.x) + s1.x * (a.z - c.z)) / denominator;
	float r = (s2.x * (a.z - c.z) - s2.z * (a.x - c.x)) / denominator;

	if (r >= 0 && r <= 1 && s >= 0 && s <= 1)
	{
		pointOfIntersection = position(a.x + (r * s1.x), a.z + (r * s1.z));
		return true;
	}

	return false;
}

position Boid::WallAvoidance()
{
	std::array<position, 3> feelers;

	feelers[0] = position(1, 0);
	feelers[1] = position(1, 0);
	feelers[2] = position(1, 0);

	float length = 1.0f;

	float s = sin(-rotation * M_PI / 180);
	float c = cos(-rotation * M_PI / 180);
	feelers[0] = position(pos.x + length * c, pos.z + length * s);

	s = sin(-(rotation + 45) * M_PI / 180);
	c = cos(-(rotation + 45) * M_PI / 180);
	feelers[1] = position(pos.x + length * c, pos.z + length * s);

	s = sin(-(rotation - 45) * M_PI / 180);
	c = cos(-(rotation - 45) * M_PI / 180);
	feelers[2] = position(pos.x + length * c, pos.z + length * s);

	glBegin(GL_LINES);
		glVertex3f(pos.x, 1.0f, pos.z);
		glVertex3f(feelers[0].x, 1.0f, feelers[0].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(pos.x, 1.0f, pos.z);
		glVertex3f(feelers[1].x, 1.0f, feelers[1].z);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(pos.x, 1.0f, pos.z);
		glVertex3f(feelers[2].x, 1.0f, feelers[2].z);
	glEnd();

	//IP == Intersection Point
	float distToIP = 0.0f;
	float distToClosestIP = 9999999;

	int closestWall = -1;

	position steeringForce = position();
	position closestPoint = position();

	for (position feeler : feelers)
	{
		for (int i = 0; i < walls->size(); i++)
		{
			position point;
			if (lineIntersection(walls->at(i).start, walls->at(i).end, feeler, pos, point))
			{
				float mag = sqrt((pos.x - point.x)*(pos.x - point.x) + (pos.z - point.z)*(pos.z - point.z));

				if (mag < distToClosestIP)
				{
					distToClosestIP = mag;
					closestWall = i;
					closestPoint = point;
				}
			}
		}

		if (closestWall != -1)
		{
			position overshoot = feeler;
			overshoot.x -= closestPoint.x;
			overshoot.z -= closestPoint.z;
			
			float overshootLength = sqrt(overshoot.x * overshoot.x + overshoot.z * overshoot.z);
			//position normal = position(-(walls->at(closestWall).end.z - walls->at(closestWall).start.z), (walls->at(closestWall).end.x - walls->at(closestWall).start.x));
			//normal = normalise(normal);
			//steeringForce = position(normal.x * overshootLength, normal.z * overshootLength);

			float areaHeight, areaWidth;
			for (int i = 0; i < walls->size(); i++)
			{
				if (walls->at(i).start.x == walls->at(i).end.x)
				{
					areaWidth = walls->at(i).end.z - walls->at(i).start.z;
				}

				if (walls->at(i).start.z == walls->at(i).end.z)
				{
					areaHeight = walls->at(i).end.x - walls->at(i).start.x;
				}
			}

			areaHeight = abs(areaHeight);
			areaWidth = abs(areaWidth);

			//Vertical wall
			if (walls->at(closestWall).start.x == walls->at(closestWall).end.x)
			{
				//Left wall
				if (walls->at(closestWall).start.z == 0 || walls->at(closestWall).end.z == 0)
				{
					if (pos.z <= areaHeight / 2)
					{
						steeringForce = position(1, 5);
					}
					else
					{
						steeringForce = position(1, -5);
					}
				}
				else //Right wall
				{
					if (pos.z <= areaHeight / 2)
					{
						steeringForce = position(-1, 5);
					}
					else
					{
						steeringForce = position(-1, -5);
					}
				}
			}
			else
			{
				//Top wall
				if (walls->at(closestWall).start.x == 0 || walls->at(closestWall).end.x == 0)
				{
					if (pos.x >= areaWidth / 2)
					{
						steeringForce = position(-5, 1);
					}
					else
					{
						steeringForce = position(5, 1);
					}
				}
				else //Bottom wall
				{
					if (pos.x >= areaWidth / 2)
					{
						steeringForce = position(-5, -1);
					}
					else
					{
						steeringForce = position(5, -1);
					}
				}
			}

			overshootLength *= 10;
			//steeringForce = normalise(steeringForce);
			steeringForce = position(steeringForce.x * overshootLength, steeringForce.z * overshootLength);
		}
	}
	
	return steeringForce;
}

position Boid::aggregateSteering(position& wander, position& separate, position& align, position& cohese, position& avoidWalls)
{
	position steerForce = position(0, 0);

	steerForce.x += wander.x * wanderWeight;
	steerForce.z += wander.z * wanderWeight;

	steerForce.x += separate.x * separateWeight;
	steerForce.z += separate.z * separateWeight;

	steerForce.x += align.x * alignWeight;
	steerForce.z += align.z * alignWeight;

	steerForce.x += cohese.x * coheseWeight;
	steerForce.z += cohese.z * coheseWeight;

	steerForce.x += avoidWalls.x * avoidWallWeight;
	steerForce.x += avoidWalls.z * avoidWallWeight;

	return steerForce;
}

std::vector<BoidInfo> Boid::getNeighbourhood(const std::vector<BoidInfo>& allBoids)
{
	float radius = 2;
	float fov = 180;
	std::vector<BoidInfo> neighbours;

	for (BoidInfo boid : allBoids)
	{
		if (boid.id == id)
		{
			continue;
		}

		float distanceSq = (boid.pos.x - pos.x) * (boid.pos.x - pos.x) + (boid.pos.z - pos.z) * (boid.pos.z - pos.z);
		if (distanceSq > radius * radius)
		{
			continue;
		}

		position vecToOther = pos;
		vecToOther.x -= boid.pos.x;
		vecToOther.z -= boid.pos.z;

		vecToOther = normalise(vecToOther);
		position heading = normalise(velocity);

		float theta = acos((vecToOther.x * heading.x) + (vecToOther.z * heading.z)) * 180 / M_PI;
		if (theta < fov || theta > 360 - fov)
		{
			neighbours.push_back(boid);
		}
	}
	return neighbours;
}
#include <Windows.h>

void Boid::Update(float delta, const std::vector<BoidInfo>& others)
{
	delta = 0.03f;

	
	std::vector<BoidInfo> neighbours = getNeighbourhood(others);
	position wander = Wander();

	position separate = Separation(neighbours);

	position alignment = Alignment(neighbours);

	position cohese = Cohesion(neighbours);
	
	position avoidWalls = WallAvoidance();

	position steeringForce = aggregateSteering(wander, separate, alignment, cohese, avoidWalls);	

	//Update position based on old velocity and accelleration (mass is 1)
	pos.x += (velocity.x * delta) + 0.5f * (steeringForce.x * (delta * delta));
	pos.z += (velocity.z * delta) + 0.5f * (steeringForce.z * (delta * delta));

	//Update velocity
	velocity.x += steeringForce.x * delta;
	velocity.z += steeringForce.z * delta;
	velocity = truncate(velocity, maxVelocity);

	//Update rotation
	position heading = normalise(velocity);
	float deg = atan2(-heading.z, heading.x) * 180 / M_PI;
	rotation = deg;

	if (GetAsyncKeyState(VK_RIGHT))
	{
		pos.x += 0.1;
	}else if (GetAsyncKeyState(VK_LEFT))
	{
		pos.x -= 0.1;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		pos.z -= 0.1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		pos.z += 0.1;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		rotation += 1;
	}
}

void Boid::Render()
{
	glPushMatrix();
		glTranslatef(pos.x, 0.5f, pos.z);
		glRotatef(rotation, 0, 1, 0);
		glColor3f(red, green, blue);
		glutSolidTeapot(0.25);
	glPopMatrix();
}

void Boid::resolveCollision(position moveBy)
{
	pos.x += moveBy.x;
	pos.z += moveBy.z;
}
