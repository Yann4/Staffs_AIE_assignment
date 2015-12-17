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

Boid::Boid(char id, position pos, Graph* g, std::vector<Wall>* walls) : pos(pos), id(id), walls(walls)
{
	red = 0;
	green = 255;
	blue = 0;

	rotation = 0;

	velocity = position(0, 0);
	wanderTarget = pos;

	width = 0.25;
	height = 0.5;

	giveGoodGuyFSM();
	currentState->Enter();
}

void Boid::giveGoodGuyFSM()
{
	currentState = new EscapeState(this, g, new FlockState(this, nullptr));
}

void Boid::giveBadGuyFSM()
{

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
	float wanderJitter = 3;
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
	float feelerAngle = 50;
	s = sin(-(rotation + feelerAngle) * M_PI / 180);
	c = cos(-(rotation + feelerAngle) * M_PI / 180);
	feelers[1] = position(pos.x + length * c, pos.z + length * s);

	s = sin(-(rotation - feelerAngle) * M_PI / 180);
	c = cos(-(rotation - feelerAngle) * M_PI / 180);
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
	std::vector<Wall> intersectingWalls;

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
					intersectingWalls.push_back(walls->at(i));
					closestPoint = point;
				}
			}
		}

		if (closestWall >= 0)
		{
			position overshoot = feeler;
			overshoot.x -= closestPoint.x;
			overshoot.z -= closestPoint.z;

			float multiplier = 15;
			
			float overshootLength = sqrt(overshoot.x * overshoot.x + overshoot.z * overshoot.z) * multiplier;
			position normal = position(-(walls->at(closestWall).end.z - walls->at(closestWall).start.z), (walls->at(closestWall).end.x - walls->at(closestWall).start.x));
			normal = normalise(normal);
			steeringForce = position((normal.x * overshootLength), (normal.z * overshootLength * 5));
		}
		closestWall = -1;
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
	steerForce.z += avoidWalls.z * avoidWallWeight;

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

void Boid::givePath(std::stack<position> path)
{
	this->path = path;
}

position Boid::followPath()
{
	float radius = 0.5f;
	radius *= radius;
	
	if (!path.empty())
	{
		position nextNode = path.top();

		float distSq = (pos.x - nextNode.x) * (pos.x - nextNode.x) + (pos.z - nextNode.z) * (pos.z - nextNode.z);
		distSq = abs(distSq);

		if (distSq < radius)
		{
			path.pop();
			if (!path.empty())
			{
				nextNode = path.top();
			}
		}
		return Seek(nextNode);
	}
	return position();
}

void Boid::Update(float delta, const std::vector<BoidInfo>& others)
{
	delta = 0.03;

	currentState->Update(delta, others);
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

void Boid::UpdateLocation(position steeringForce, float delta)
{
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
}

void Boid::resolveCollision(position moveBy)
{
	pos.x += moveBy.x;
	pos.z += moveBy.z;
}
