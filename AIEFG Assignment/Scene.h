#ifndef _SCENE_H_
#define _SCENE_H_

#include <windows.h>
#include <cstdio>
#include <GL\gl.h>
#include <GL\glu.h>
#include "gl/glut.h"
#include "Obstacle.h"
#include "ObjectTypes.h"
#include <vector>
#include "Boid.h"
#include "Graph.h"
#include "MathHelper.h"

class Scene
{
private:
	Obstacle**	 m_pWalls;
	unsigned int m_iWallQty;
	position	 m_ScenarioOffset;

	std::vector<Boid*> boids;

	std::vector<Boid*> enemies;

	std::vector<Wall> walls;
	Graph* graph;

	bool renderGraph;
	int resetCounter;
	int boidNum;

public:
	Scene();
	~Scene();

	bool	Initialise();
	void	Render();
	void	Update(int a_deltaTime);								
	
private:
	void	SetUpScenario();
	void	DrawScenario();
	void    UpdateScenario(int a_deltaTime);
};

#endif //_SCENE_H_
