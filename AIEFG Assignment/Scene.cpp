#include "Scene.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "Boid.h"
#include <string>

//--------------------------------------------------------------------------------------------------------

Scene::Scene()
{
	//Current scenario
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;

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

	return;
}

//--------------------------------------------------------------------------------------------------------

bool Scene::Initialise()
{
	glEnable(GL_DEPTH_TEST);

	SetUpScenario();
	
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

	position p;
	p.x = 9.5;
	p.z = 20;

	Boid b = Boid(p, 90.0f);

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

	b.Render();
	//Draw the Scenario.
	DrawScenario();
}

//--------------------------------------------------------------------------------------------------------

void Scene::Update(int a_deltaTime)
{
	//Update the Scenario.
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

	//Setup your edge list here.
}

//--------------------------------------------------------------------------------------------------------

void Scene::UpdateScenario(int a_deltaTime)
{
}

//--------------------------------------------------------------------------------------------------------