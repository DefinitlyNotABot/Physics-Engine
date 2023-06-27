#include <iostream>
#include <cmath>

#include "../Header_Files/Softbody.h"


Softbody::Softbody(vec2 pos, sfCol col, float stiff, float mass, int w, int h, int res, float damp)
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
	type = PH_SOF;

	createSoftbody();
}

void Softbody::draw(sf::RenderWindow* window)
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

void Softbody::createSoftbody()
{

	float dis_w = width / resolution;
	float dis_h = height / resolution;

	for (int j = 0; j < resolution; j++)
	{
		for (int i = 0; i < resolution; i++)
		{


			Masspoint* m = new Masspoint(vec2(position.x + dis_w * i, position.y + dis_h * j), masses, 2);
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

		for (int i = -1; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{

				if (x + i >= 0 && x + i < resolution && y + j >= 0 && y + j < resolution && (i != 0 || j != 0) && (i != -1 || j != 0))
				{
					itY = itX;
					int z = (y + j) * resolution + (x + i);
					int rel = z - ind;
					std::advance(itY, rel);
					Spring* spr = new Spring(*itX, *itY, stiffness, dampness);
					springs.push_back(spr);
				}
			}
		}
		ind++;
	}

}




void Softbody::physicsStep(int chunk_id)
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
			p->physicsStep(delta);
		}



		t0 = Time::now();
	}

}

void Softbody::collision(PhysicsObject& p)
{
	// Calculate the distance between the centers of the two particles
	switch (p.type)
	{
	case PH_PAR:
		break;
	case PH_TRI:
	{




	}
	break;
	}

}

void Softbody::collisionUpdatePos()
{

}

