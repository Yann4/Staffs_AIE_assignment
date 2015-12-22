#include "Scene.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include <string>

//--------------------------------------------------------------------------------------------------------

Scene::Scene()
{
	//Current scenario
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;

	//Actual walls
	walls.push_back(Wall(position(0, 12), position(0, 0)));
	walls.push_back(Wall(position(0, 0), position(21, 0)));
	walls.push_back(Wall(position(21, 0), position(21, 12)));
	walls.push_back(Wall(position(21, 12), position(0, 12)));

	renderGraph = false;

	return;
}

//--------------------------------------------------------------------------------------------------------

Scene::~Scene()
{
	//Delete the memory and references.

	//Scenario 
	if(m_pWalls != NULL)
	{
		for(unsigned int i=0; i < m_iWallQty; i++)
		{
			delete *(m_pWalls+i);
			*(m_pWalls+i) = NULL;
		}
		delete [] m_pWalls;
		m_pWalls = NULL;
	}

	delete graph;

	for (Boid* b : enemies)
	{
		delete b;
	}

	for (Boid* b : boids)
	{
		delete b;
	}

	return;
}

//--------------------------------------------------------------------------------------------------------

bool Scene::Initialise()
{
	glEnable(GL_DEPTH_TEST);

	SetUpScenario();

	position mazeEntrypoint = position(9.5, 20);
	boids.push_back(new Boid(boids.size(), mazeEntrypoint, graph, &walls));

	position e11 = position(1.5, 18.5);
	position e12 = position(7.5, 18.5);

	position e21 = position(11.5, 18.5);
	position e22 = position(19.5, 18.5);

	position e31 = position(1.5, 13.5);
	position e32 = position(15.5, 13.5);

	enemies.push_back(new Boid('e', e31, graph, &walls));
	enemies.push_back(new Boid('f', e21, graph, &walls));
	enemies.at(0)->makeBadGuy(graph, e31, e32);
	enemies.at(1)->makeBadGuy(graph, e21, e22);

	resetCounter = 0;

	return true;
}

//--------------------------------------------------------------------------------------------------------

void Scene::Render()
{
	// Draw a gridded ground
    float length  = 21.0f;
    float spacing = 1.0f;
    float yPos	  = 0.0f;
    float iLine;
    //Change colour to yellow.
	glColor3f(0.6f,0.5f,0.2f);

    glBegin(GL_LINES);
       for(iLine = 0; iLine <= length; iLine += spacing)
       {
			//Draw forward lines
			glVertex3f(iLine, yPos, length);
			glVertex3f(iLine, yPos, 0);
    
			//Draw crossing lines
			glVertex3f(length, yPos, iLine);
			glVertex3f(0, yPos, iLine);
       }
    glEnd();

	//Draw the Scenario.
	DrawScenario();
}

//--------------------------------------------------------------------------------------------------------

void Scene::Update(int a_deltaTime)
{
	//Update the Scenario.

	//Debug render options
	if (GetAsyncKeyState(VK_F2) & 1)
	{
		bool state = boids.at(0)->getWhiskerState();
		for (Boid* b : boids)
		{
			b->setWhiskerRender(!state);
		}
	}
	if (GetAsyncKeyState(VK_F3) & 1)
	{
		renderGraph = !renderGraph;
	}

	UpdateScenario(a_deltaTime);
}

//--------------------------------------------------------------------------------------------------------

void Scene::DrawScenario()
{
	//Walls
	for (unsigned int i = 0; i < m_iWallQty; i++)
	{
		m_pWalls[i]->Render();
	}

	for (Boid* b : enemies)
	{
		b->Render();
	}

	for (Boid* b : boids)
	{
		b->Render();
	}

	if (renderGraph)
	{
		graph->RenderGraph();
		graph->resetGraph();
	}
}

//Methods to set up pointer arrays to all the wall pieces.
void Scene::SetUpScenario()
{
	//Set amount of obstacles / walls for this scenario.
	m_iWallQty = SCENARIO_WALL_QUANTITY;

	//Set no offset, so drawing takes place in corner.
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;
			
	//Set up walls.
	m_pWalls = new Obstacle *[m_iWallQty];
	m_pWalls[0] = new Obstacle(0, 0.5, 20);
	m_pWalls[1] = new Obstacle(1, 0.5, 20);
	m_pWalls[2] = new Obstacle(2, 0.5, 20);
	m_pWalls[3] = new Obstacle(3, 0.5, 20);
	m_pWalls[4] = new Obstacle(4, 0.5, 20);
	m_pWalls[5] = new Obstacle(5, 0.5, 20);
	m_pWalls[6] = new Obstacle(6, 0.5, 20);
	m_pWalls[7] = new Obstacle(7, 0.5, 20);
	m_pWalls[8] = new Obstacle(8, 0.5, 20);
	m_pWalls[9] = new Obstacle(10, 0.5, 20);
	m_pWalls[10] = new Obstacle(11, 0.5, 20);
	m_pWalls[11] = new Obstacle(12, 0.5, 20);
	m_pWalls[12] = new Obstacle(13, 0.5, 20);
	m_pWalls[13] = new Obstacle(14, 0.5, 20);
	m_pWalls[14] = new Obstacle(15, 0.5, 20);
	m_pWalls[15] = new Obstacle(16, 0.5, 20);
	m_pWalls[16] = new Obstacle(17, 0.5, 20);
	m_pWalls[17] = new Obstacle(18, 0.5, 20);
	m_pWalls[18] = new Obstacle(19, 0.5, 20);
	m_pWalls[19] = new Obstacle(20, 0.5, 20);

	m_pWalls[20] = new Obstacle(0, 0.5, 19);
	m_pWalls[21] = new Obstacle(8, 0.5, 19);
	m_pWalls[22] = new Obstacle(10, 0.5, 19);
	m_pWalls[23] = new Obstacle(20, 0.5, 19);

	m_pWalls[24] = new Obstacle(0, 0.5, 18);
	m_pWalls[25] = new Obstacle(2, 0.5, 18);
	m_pWalls[26] = new Obstacle(3, 0.5, 18);
	m_pWalls[27] = new Obstacle(4, 0.5, 18);
	m_pWalls[28] = new Obstacle(6, 0.5, 18);
	m_pWalls[29] = new Obstacle(12, 0.5, 18);
	m_pWalls[30] = new Obstacle(13, 0.5, 18);
	m_pWalls[31] = new Obstacle(14, 0.5, 18);
	m_pWalls[32] = new Obstacle(16, 0.5, 18);
	m_pWalls[33] = new Obstacle(17, 0.5, 18);
	m_pWalls[34] = new Obstacle(18, 0.5, 18);
	m_pWalls[35] = new Obstacle(19, 0.5, 18);
	m_pWalls[36] = new Obstacle(20, 0.5, 18);

	m_pWalls[37] = new Obstacle(0, 0.5, 17);
	m_pWalls[38] = new Obstacle(1, 0.5, 17);
	m_pWalls[39] = new Obstacle(2, 0.5, 17);
	m_pWalls[40] = new Obstacle(4, 0.5, 17);
	m_pWalls[41] = new Obstacle(6, 0.5, 17);
	m_pWalls[42] = new Obstacle(7, 0.5, 17);
	m_pWalls[43] = new Obstacle(8, 0.5, 17);
	m_pWalls[44] = new Obstacle(9, 0.5, 17);
	m_pWalls[45] = new Obstacle(10, 0.5, 17);
	m_pWalls[46] = new Obstacle(11, 0.5, 17);
	m_pWalls[47] = new Obstacle(12, 0.5, 17);
	m_pWalls[48] = new Obstacle(14, 0.5, 17);
	m_pWalls[49] = new Obstacle(20, 0.5, 17);

	m_pWalls[50] = new Obstacle(0, 0.5, 16);
	m_pWalls[51] = new Obstacle(16, 0.5, 16);
	m_pWalls[52] = new Obstacle(17, 0.5, 16);
	m_pWalls[53] = new Obstacle(18, 0.5, 16);
	m_pWalls[54] = new Obstacle(20, 0.5, 16);

	m_pWalls[55] = new Obstacle(0, 0.5, 15);
	m_pWalls[56] = new Obstacle(1, 0.5, 15);
	m_pWalls[57] = new Obstacle(2, 0.5, 15);
	m_pWalls[58] = new Obstacle(3, 0.5, 15);
	m_pWalls[59] = new Obstacle(4, 0.5, 15);
	m_pWalls[60] = new Obstacle(5, 0.5, 15);
	m_pWalls[61] = new Obstacle(6, 0.5, 15);
	m_pWalls[62] = new Obstacle(7, 0.5, 15);
	m_pWalls[63] = new Obstacle(8, 0.5, 15);
	m_pWalls[64] = new Obstacle(10, 0.5, 15);
	m_pWalls[65] = new Obstacle(11, 0.5, 15);
	m_pWalls[66] = new Obstacle(12, 0.5, 15);
	m_pWalls[67] = new Obstacle(13, 0.5, 15);
	m_pWalls[68] = new Obstacle(14, 0.5, 15);
	m_pWalls[69] = new Obstacle(15, 0.5, 15);
	m_pWalls[70] = new Obstacle(16, 0.5, 15);
	m_pWalls[71] = new Obstacle(20, 0.5, 15);

	m_pWalls[72] = new Obstacle(0, 0.5, 14);
	m_pWalls[73] = new Obstacle(16, 0.5, 14);
	m_pWalls[74] = new Obstacle(18, 0.5, 14);
	m_pWalls[75] = new Obstacle(20, 0.5, 14);

	m_pWalls[76] = new Obstacle(0, 0.5, 13);
	m_pWalls[77] = new Obstacle(1, 0.5, 13);
	m_pWalls[78] = new Obstacle(3, 0.5, 13);
	m_pWalls[79] = new Obstacle(4, 0.5, 13);
	m_pWalls[80] = new Obstacle(5, 0.5, 13);
	m_pWalls[81] = new Obstacle(6, 0.5, 13);
	m_pWalls[82] = new Obstacle(7, 0.5, 13);

	m_pWalls[83] = new Obstacle(8, 0.5, 13);
	m_pWalls[84] = new Obstacle(9, 0.5, 13);
	m_pWalls[85] = new Obstacle(10, 0.5, 13);
	m_pWalls[86] = new Obstacle(11, 0.5, 13);
	m_pWalls[87] = new Obstacle(12, 0.5, 13);
	m_pWalls[88] = new Obstacle(13, 0.5, 13);
	m_pWalls[89] = new Obstacle(14, 0.5, 13);
	m_pWalls[90] = new Obstacle(15, 0.5, 13);
	m_pWalls[91] = new Obstacle(16, 0.5, 13);
	m_pWalls[92] = new Obstacle(18, 0.5, 13);
	m_pWalls[93] = new Obstacle(19, 0.5, 13);
	m_pWalls[94] = new Obstacle(20, 0.5, 13);

	//Set all to color and offset.
	for(unsigned int i = 0; i < m_iWallQty; i++)
	{
		m_pWalls[i]->SetColor(1.0f, 0.0f, 0.0f);
		m_pWalls[i]->SetOffset(m_ScenarioOffset);
	}

	//Add your waypoint structure here.
	graph = new Graph(position(0.5, 0.5), 1, m_pWalls, m_iWallQty);
}

//--------------------------------------------------------------------------------------------------------

void Scene::UpdateScenario(int a_deltaTime)
{
	std::vector<BoidInfo> info;

	for (Boid* b : boids)
	{
		info.push_back(b->getInfo());
	}

	if (info.size() > 0 && info.back().target)
	{
		position mazeEntrypoint = position(9.5, 20);
		boids.push_back(new Boid(boids.size(), mazeEntrypoint, graph, &walls));
	}
	else if (info.size() == 0)
	{
		position mazeEntrypoint = position(9.5, 20);
		boids.push_back(new Boid(boids.size(), mazeEntrypoint, graph, &walls));
	}

	for (Boid* b : boids)
	{
		b->Update(a_deltaTime, info);
	}

	for (Boid* b : enemies)
	{
		b->Update(a_deltaTime, info);
		b->ExertInfluence(graph);
		std::vector<char> idToDelete;
		for (Boid* victim : boids)
		{
			Collision::MTV mtv;
			if (Collision::collision(b->getBoundingBox(), victim->getBoundingBox(), mtv))
			{
				idToDelete.push_back(victim->getInfo().id);
			}
		}

		for (char c : idToDelete)
		{
			int index = 0;

			for (index = 0; index < boids.size(); index++)
			{
				if (boids.at(index)->getInfo().id == c)
				{
					break;
				}
			}

			boids.erase(boids.begin() + index);
			b->ateTeapot();
		}
	}
}