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
	width = 0.5;
	height = 1;
}

Boid::Boid(position pos) : pos(pos)
{
	red = 0;
	green = 255;
	blue = 0;

	rotation = 0;

	velocity = position(0, 0);
	wanderTarget = pos;

	width = 0.5;
	height = 1;
}

float randomInRange(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

position normalise(position p)
{
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

position Boid::aggregateSteering(position & wander)
{
	position steerForce = position(0, 0);

	steerForce.x += wander.x * wanderWeight;
	steerForce.z += wander.z * wanderWeight;

	return steerForce;
}

void Boid::Update(float delta)
{
	delta = 0.01f;

	position wander = Wander();

	position steeringForce = aggregateSteering(wander);

	//Update position based on old velocity and accelleration (mass is 1)
	pos.x += (velocity.x * delta) + 0.5f * (steeringForce.x * (delta * delta));
	pos.z += (velocity.z * delta) + 0.5f * (steeringForce.z * (delta * delta));

	//Update velocity
	velocity.x += steeringForce.x * delta;
	velocity.z += steeringForce.z * delta;
	velocity = truncate(velocity, maxVelocity);

	//Update rotation
	position heading = normalise(velocity);
	float rads = atan2(-heading.z, heading.x);
	float deg = rads * 180 / M_PI;
	rotation = deg;
}

void Boid::Render()
{
	glPushMatrix();
		glTranslatef(pos.x, 0.5f, pos.z);
		glRotatef(rotation, 0, 1, 0);
		glColor3f(red, green, blue);
		glutSolidTeapot(0.5);
	glPopMatrix();
}

void Boid::resolveCollision(position moveBy)
{
	pos.x += moveBy.x;
	pos.z += moveBy.z;
}
