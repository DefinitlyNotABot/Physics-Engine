#include <iostream>
#include <cmath>

#include "../Header_Files/Triangle.h"

sf::Vector2f vec2_2_sfVec2(glm::vec2* x)
{
	return sf::Vector2f(x->x, x->y);
}

sfCol bgCol3 = sfCol(0, 0, 0);

Triangle::Triangle(vec2 pos, vec2 pts[3], sfCol col, bool isHollow, float bouncy, float m)
{
	for (int i = 0; i < 3; i++) {
		points[i] = pts[i];
	}

	position = pos;
	color = col;
	hollow = isHollow;
	bouncyness = bouncy;
	createTriangle();

	mass = m;

	moveDir = vec2(0, 0);

	ID = nextID;
	nextID++;

	t0 = Time::now();
	
}

void Triangle::draw(sf::RenderWindow* window)
{
	std::cout << "should draw" << std::endl;
	window->draw(triangle);
	
}

void Triangle::createTriangle()
{
	triangle = sf::VertexArray(sf::Triangles, 3);

	for (int i = 0; i < 3; i++)
	{
		points[i] += position;
		triangle[i].position = vec2_2_sfVec2(&points[i]);
		triangle[i].color = color;
	}

}


void Triangle::physicsStep()
{
	if (!step_calculated)
	{
		
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

