#include "Boid.h"
#include <iostream>

Boid::Boid()
{
	pos.x = 0;
	pos.z = 0;
	rotation = 0;
	boidShape = TEAPOT;
	red = 0;
	green = 255;
	blue = 0;
	velocity.x = 0;
	velocity.z = 0;

	wanderTarget.x = 0;
	wanderTarget.z = 0;
}

Boid::Boid(position pos, float rotation) : pos(pos), rotation(rotation), boidShape(TEAPOT)
{
	red = 0;
	green = 255;
	blue = 0;
	velocity.x = 0;
	velocity.z = 0;

	wanderTarget = pos;
}

float randomInRange(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

position Normalise(position p)
{
	float mag = sqrtf(p.x * p.x + p.z * p.z);
	p.x /= mag;
	p.z /= mag;
	return p;
}

position Truncate(position p, float maxMag)
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

	wanderTarget = Normalise(wanderTarget);

	wanderTarget.x *= wanderRadius;
	wanderTarget.z *= wanderRadius;

	position targetLocal = wanderTarget;
	targetLocal.x += wanderDistance;

	position targetWorld;
	targetWorld.x = pos.x + targetLocal.x;
	targetWorld.z = pos.z + targetLocal.z;
	targetWorld.x += cos(rotation) * (targetLocal.x) - sin(rotation) * (targetLocal.z);
	targetWorld.z += sin(rotation) * (targetLocal.x) - cos(rotation) * (targetLocal.z);


	std::cout << "pos world: " << targetWorld.x << ", " << targetWorld.z << std::endl;

	position wanderForce = targetWorld;
	wanderForce.x -= pos.x;
	wanderForce.z -= pos.z;

	return wanderForce;
}

void Boid::Update(float delta)
{
	delta = 0.01f;

	position wander = Wander();
	std::cout << "wander: " << wander.x << ", " << wander.z << std::endl;

	pos.x += (velocity.x * delta) + 0.5f * (wander.x * (delta * delta));
	pos.z += (velocity.z * delta) + 0.5f * (wander.z * (delta * delta));
	std::cout << "pos: " << pos.x << ", " << pos.z << std::endl;

	velocity.x += wander.x * delta;
	velocity.z += wander.z * delta;

	velocity = Truncate(velocity, maxVelocity);
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