#pragma once
#include "ObjectTypes.h"
#include "gl\glut.h"
#include "Enums.h"

class Boid
{
private:
	SHAPE boidShape;
	position pos;
	float rotation;
	float red, green, blue;

public:
	Boid();
	Boid(position pos, float rotation);

	void Update();
	void Render();
};