#include "Boid.h"

Boid::Boid()
{
	pos.x = 0;
	pos.z = 0;
	rotation = 0;
	boidShape = TEAPOT;
	red = 0;
	green = 255;
	blue = 0;

	wanderTarget = position(0, 0);
	vel = position(0, 0);
}

Boid::Boid(position pos, float rotation) : pos(pos), rotation(rotation), boidShape(TEAPOT)
{
	red = 0;
	green = 255;
	blue = 0;

	wanderTarget = position(0, 0);
	vel = position(0, 0);
}

float randomClamped()
{
	return -1 + static_cast<float>(rand()) / (static_cast<float>(0.5f));
}

Force clamp(Force p)
{
	float mag = p.x * p.x + p.y * p.y;
	mag = sqrt(mag);
	float maxSpeed = 0.1;

	if (mag > maxSpeed)
	{
		p.x /= mag;
		p.y /= mag;
		p.x *= maxSpeed;
		p.y *= maxSpeed;
	}
	return p;
}

position normalise(position pos)
{
	float mag = pos.x * pos.x + pos.z * pos.z;
	mag = sqrt(mag);

	pos.x /= mag;
	pos.z /= mag;
	return pos;
}

Force Boid::wander()
{
	float wanderRadius = 1;
	float wanderDistance = 1;
	float wanderJitter = 1;

	wanderTarget = position(wanderTarget.x + randomClamped() * wanderJitter, wanderTarget.z + randomClamped() * wanderJitter);
	wanderTarget = normalise(wanderTarget);

	wanderTarget.x *= wanderRadius;
	wanderTarget.z *= wanderRadius;

	position targetLocal = position(wanderTarget.x + wanderDistance, wanderTarget.z);

	position targetWorld;
	//Rotate point
	targetWorld.x = targetLocal.x * cos(rotation) - targetLocal.x * sin(rotation);
	targetWorld.z = targetLocal.x * sin(rotation) - targetLocal.z * cos(rotation);

	//Translate point
	targetWorld.x += pos.x;
	targetWorld.z += pos.z;

	return Force(targetWorld.x - pos.x, targetWorld.z - pos.z);
}

void Boid::Update(int deltaTime)
{
	deltaTime = 1;
	Force netForce = Force();
	Force wanderForce = wander();

	netForce.x += wanderForce.x;
	netForce.y += wanderForce.y;

	netForce = clamp(netForce);
	netForce.x = -netForce.x;
	netForce.y = -netForce.y;
	pos.x = (pos.x * deltaTime) + 0.5f * netForce.x * deltaTime * deltaTime;
	pos.z = (pos.z * deltaTime) + 0.5f * netForce.y * deltaTime * deltaTime;
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