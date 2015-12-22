#include "Boid.h"
#include <iostream>


Boid::Boid()
{
	id = -1;
	isTarget = false;
	pos = position();
	velocity = position();

	rotation = 0;
	colour = Colour();

	width = 0;
	height = 0;

	renderWhiskers = false;

	wanderTarget = position();

	walls = nullptr;

	path = std::stack<position>();

	currentState = nullptr;

	flock = nullptr;
	obstacle = nullptr;
	escape = nullptr;
	patrol = nullptr;
	hunt = nullptr;

}

Boid::Boid(int id, position pos, Graph* g, std::vector<Wall>* walls) : pos(pos), id(id), walls(walls)
{
	isTarget = false;
	velocity = position();

	rotation = 0;
	colour = Colour(0, 255, 0);

	width = 0.25f;
	height = 0.5f;

	wanderTarget = position();

	path = std::stack<position>();

	currentState = nullptr;

	flock = nullptr;
	obstacle = nullptr;
	escape = nullptr;
	patrol = nullptr;
	hunt = nullptr;

	giveGoodGuyFSM(g);
	currentState->Enter();

	renderWhiskers = false;
}

Boid::~Boid()
{
	currentState = nullptr;

	if(flock != nullptr) delete flock;
	if(obstacle != nullptr) delete obstacle;
	if(escape != nullptr) delete escape;
	if(patrol != nullptr) delete patrol;
	if(hunt != nullptr) delete hunt;

	walls = nullptr;
}

void Boid::makeBadGuy(Graph* graph, position patrolLoc1, position patrolLoc2)
{
	colour = Colour(0, 0, 255);

	delete flock;
	delete obstacle;
	delete escape;
	delete patrol;
	delete hunt;
	currentState = nullptr;

	giveBadGuyFSM(graph, patrolLoc1, patrolLoc2);
	currentState->Enter();
}

void Boid::setWhiskerRender(bool whiskers)
{
	renderWhiskers = whiskers;
}

void Boid::giveGoodGuyFSM(Graph* g)
{
	obstacle = new BecomeObstacleState(this, g);
	flock = new FlockState(this, obstacle);
	escape = new EscapeState(this, g, flock);

	currentState = escape;
}

void Boid::giveBadGuyFSM(Graph* g, position loc1, position loc2)
{
	hunt = new HuntState(this, nullptr);
	escape = new EscapeState(this, g, hunt, false);
	patrol = new PatrolState(this, escape, g, loc1, loc2);
	hunt->giveSister(patrol);
	currentState = patrol;
}

position Boid::Seek(position seekTo)
{
	position desiredVel = seekTo - pos;
	
	desiredVel = normalise(desiredVel);

	desiredVel *= maxVelocity;

	desiredVel -= velocity;

	return desiredVel;
}

position Boid::Wander()
{
	float wanderJitter = 2;
	float wanderDistance = 2;
	float wanderRadius = 10;

	wanderTarget += position(randomInRange(-1, 1) * wanderJitter, randomInRange(-1, 1) * wanderJitter);

	wanderTarget = normalise(wanderTarget);

	wanderTarget *= wanderRadius;

	position targetLocal = wanderTarget;
	targetLocal.x += wanderDistance;

	position targetWorld;

	targetWorld.x = pos.x + targetLocal.x;
	targetWorld.z = pos.z + targetLocal.z;

	targetWorld.x += cos(rotation) * (targetLocal.x) - sin(rotation) * (targetLocal.z);
	targetWorld.z += sin(rotation) * (targetLocal.x) - cos(rotation) * (targetLocal.z);

	position wanderForce = targetWorld - pos;

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

		steeringForce += neighbour.velocity;
		count++;
	}

	if (count > 0)
	{
		steeringForce /= count;

		steeringForce -= velocity;
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

		centreOfMass += neighbour.pos;
		count++;
	}

	if (count > 0)
	{
		centreOfMass /= count;

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
		position toBoid = pos - neighbour.pos;

		float dist = sqrt(toBoid.x * toBoid.x + toBoid.z * toBoid.z);
		toBoid = normalise(toBoid);
		toBoid /= dist;

		steeringForce += toBoid;
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

	if (renderWhiskers)
	{
		glColor3f(1.0, 1.0, 1.0);
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
	}

	//IP == Intersection Point
	float distToIP = 0.0f;
	float distToClosestIP = 9999999;

	int closestWall = -1;
	std::vector<Wall> intersectingWalls;

	position steeringForce = position();
	position closestPoint = position();

	for (position feeler : feelers)
	{
		for (unsigned int i = 0; i < walls->size(); i++)
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
			position overshoot = feeler - closestPoint;

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

	steerForce += wander * wanderWeight;

	steerForce += separate * separateWeight;

	steerForce += align * alignWeight;

	steerForce += cohese * coheseWeight;

	steerForce += avoidWalls * avoidWallWeight;

	return steerForce;
}

std::vector<BoidInfo> Boid::getNeighbourhood(const std::vector<BoidInfo>& allBoids)
{
	float radius = 4;
	float fov = 360;
	std::vector<BoidInfo> neighbours;

	for (BoidInfo boid : allBoids)
	{
		if (boid.id == id || !boid.target)
		{
			continue;
		}

		float distanceSq = (boid.pos.x - pos.x) * (boid.pos.x - pos.x) + (boid.pos.z - pos.z) * (boid.pos.z - pos.z);
		if (distanceSq > radius * radius)
		{
			continue;
		}

		position vecToOther = pos - boid.pos;

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
		glColor3f(colour.r, colour.g, colour.b);
		glutSolidTeapot(0.25);
	glPopMatrix();
}

void Boid::ExertInfluence(Graph * graph)
{
	float influenceVal = 200;
	float falloff = 2;

	GraphNode* temp = graph->getNearestNode(pos);
	temp->setInfluence(influenceVal);
	influenceVal /= falloff;
	float rot = abs(rotation);

	if(rot >= -45 && rot <= 45)
	{
		while (influenceVal > 1 && temp->right != nullptr && temp->right->type != OBSTACLE)
		{
			temp = temp->right;
			temp->setInfluence(influenceVal);
			influenceVal /= falloff;
		}
	}
	else if (rot >= 46 && rot <= 135)
	{
		while (influenceVal > 1 && temp->up != nullptr && temp->up->type != OBSTACLE)
		{
			temp = temp->up;
			temp->setInfluence(influenceVal);
			influenceVal /= falloff;
		}
	}
	else if (rot >= 136 && rot <= 225)
	{
		while (influenceVal > 1 && temp->left != nullptr && temp->left->type != OBSTACLE)
		{
			temp = temp->left;
			temp->setInfluence(influenceVal);
			influenceVal /= falloff;
		}
	}
	else if (rot >= 226 && rot <= 315)
	{
		while (influenceVal > 1 && temp->down != nullptr && temp->down->type != OBSTACLE)
		{
			temp = temp->down;
			temp->setInfluence(influenceVal);
			influenceVal /= falloff;
		}
	}
}

void Boid::UpdateLocation(position steeringForce, float delta)
{
	//Update position based on old velocity and accelleration (mass is 1)
	pos += (velocity * delta) + ((steeringForce * (delta * delta) * 0.5f));

	//Update velocity
	velocity += steeringForce * delta;
	velocity = truncate(velocity, maxVelocity);

	//Update rotation
	position heading = normalise(velocity);
	float deg = atan2(-heading.z, heading.x) * 180 / M_PI;
	rotation = deg;
}

void Boid::ateTeapot()
{
	hunt->AccidentallyAte();
}

void Boid::resolveCollision(position moveBy)
{
	pos += moveBy;
}
