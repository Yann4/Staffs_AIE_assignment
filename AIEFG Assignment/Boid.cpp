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
}

Boid::Boid(position pos, float rotation) : pos(pos), rotation(rotation), boidShape(TEAPOT)
{
	red = 0;
	green = 255;
	blue = 0;
}

void Boid::Update()
{

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