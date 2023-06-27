#include <iostream>
#include <cmath>

#include "../Header_Files/Cloth.h"


Cloth::Cloth(vec2 pos, sfCol col, float stiff, float mass, int w, int h, int res, float damp)
{


	position = pos;
	color = col;
	resolution = res;


	width = w;
	height = h;
	dampness = damp;
	stiffness = stiff;



	rotationSpeed = 0.0;

	masses = mass;
	std::cout << "mass " << masses << std::endl;
	moveDir = vec2(0, 0);

	ID = nextID;
	nextID++;

	t0 = Time::now();
	step_calculated = false;
	type = PH_CLO;

	createCloth();
}

void Cloth::draw(sf::RenderWindow* window)
{
	for (auto& p : masspoints)
	{
		p->draw(window);
	}
	for (auto& p : springs)
	{
		p->draw(window);
	}


}

void Cloth::createCloth()
{

	float dis_w = width / resolution;
	float dis_h = height / resolution;

	for (int j = 0; j < resolution; j++)
	{
		for (int i = 0; i < resolution; i++)
		{


			Masspoint* m = new Masspoint(vec2(position.x + dis_w * i, position.y + dis_h * j), masses, 4);
			masspoints.push_back(m);
		}
	}

	std::list<Masspoint*>::iterator itX;
	std::list<Masspoint*>::iterator itY;

	// Iterate through the list and print each element
	int ind = 0;
	int x = 0;
	int y = 0;

	for (itX = masspoints.begin(); itX != masspoints.end(); ++itX) {

		x = ind % resolution;
		y = ind / resolution;

		

		if (x+1 < resolution)
		{
			itY = itX;
			std::advance(itY, 1);
			Spring* spr = new Spring(*itX, *itY, stiffness, dampness);
			springs.push_back(spr);
		}
		if (y + 1 < resolution)
		{
			itY = itX;
			std::advance(itY, resolution);
			Spring* spr = new Spring(*itX, *itY, stiffness, dampness);
			springs.push_back(spr);
		}
		ind++;
	}

}




void Cloth::physicsStep(int chunk_id)
{
	if (!step_calculated)
	{
		t1 = Time::now();
		fsec fs = t1 - t0;
		float delta = fs.count();


		if (DEBUG_SINGLESTEP)
		{
			delta = DEBUG_SINGLESTEP_DELTATIME;
		}

		int maxX = std::numeric_limits<int>::min();
		int minX = std::numeric_limits<int>::max();
		int maxY = std::numeric_limits<int>::min();
		int minY = std::numeric_limits<int>::max();

		for (auto& p : masspoints)
		{
			p->reset();
		}

		for (auto& p : springs)
		{
			p->physicsStep();
		}
		for (auto& p : masspoints)
		{
			if(p->ID != 0 && p->ID != resolution-1)
			p->physicsStep(delta);
			if (p->position.x > maxX)
			{
				maxX = p->position.x;
			}
			if (p->position.x < minX)
			{
				minX = p->position.x;
			}
			if (p->position.y > maxY)
			{
				maxY = p->position.y;
			}
			if (p->position.y < minY)
			{
				minY = p->position.y;
			}
		}

		up = minY;
		down = maxY;
		left = minX;
		right = maxX;



		t0 = Time::now();
	}

}

void Cloth::collision(PhysicsObject& p)
{
	// Calculate the distance between the centers of the two particles
	switch (p.type)
	{
	case PH_PAR:
	{

	}
	break;
	case PH_TRI:
	{
		for (auto& q : masspoints)
		{
			q->collision(p);
		}
	}
	break;
	case PH_SOF:
	{

	}
	break;
	}

}

void Cloth::collisionUpdatePos()
{

}

