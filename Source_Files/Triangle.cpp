#include <iostream>
#include <cmath>

#include "../Header_Files/Triangle.h"



sfCol bgCol3 = sfCol(0, 0, 0);

Triangle::Triangle(vec2 pos, vec2 pts[3], sfCol col, bool isHollow, float bouncy, float m)
{
	for (int i = 0; i < 3; i++) {
		relativePoints[i] = pts[i];
	}

	position = pos;
	color = col;
	hollow = isHollow;
	bouncyness = bouncy;
	createTriangle();

	angMomentum = 0.001;

	mass = m;

	moveDir = vec2(0, 0);

	ID = nextID;
	nextID++;

	t0 = Time::now();
	
}

void Triangle::draw(sf::RenderWindow* window)
{
	window->draw(triangle);
	
}

void Triangle::createTriangle()
{
	triangle = sf::VertexArray(sf::Triangles, 3);
	updateTriangle();
}

void Triangle::updateTriangle()
{
	for (int i = 0; i < 3; i++)
	{
		points[i] = relativePoints[i] +  position;
		triangle[i].position = vec2_2_sfVec2(&points[i]);
		triangle[i].color = color;
	}
}


void Triangle::physicsStep()
{
	if (!step_calculated)
	{
		angMomentum *= AIR_RESSISTANCE;
		step_calculated = true;
		t1 = Time::now();
		fsec fs = t1 - t0;
		float delta = fs.count();
		this->delta = delta;

		// Calculate Gravity
		moveDir.y += g * delta;
		
		// ~Calculate Gravity

		// Calculate Forces
		float forceLength = glm::length(force);
		if (forceLength > 0)
		{
			float a = forceLength / mass;
			moveDir += glm::normalize(force) * a * delta;
		}

		position += moveDir;

		for (int i = 0; i < 3; i++)
		{
			relativePoints[i] = rotate_vector(relativePoints[i], angMomentum);
		}

		updateTriangle();


		for (int i = 0; i < 3; i++)
		{
			if (points[i].y > SCREEN_HEIGHT)
			{
				position.y -= points[i].y - SCREEN_HEIGHT;
				moveDir.y *= -bouncyness;
				updateTriangle();
			}

			if (points[i].x < 0)
			{
				position.x -= points[i].x;
				moveDir.x *= -bouncyness;
				updateTriangle();
			}
			else if (points[i].x > SCREEN_WIDTH)
			{
				position.x -= points[i].x - SCREEN_WIDTH;
				moveDir.x *= -bouncyness;
				updateTriangle();
			}
		}








		moveDirSave = moveDir;
		positionSave = position;
		

		t0 = Time::now();
	}
	
}

void Triangle::collision(const Triangle& p)
{
	// Calculate the distance between the centers of the two particles
	
}

void Triangle::collisionUpdatePos()
{
	position = positionSave;
	moveDir = moveDirSave;
}

