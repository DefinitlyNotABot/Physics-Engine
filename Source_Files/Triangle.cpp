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

	int x = 0;
	int y = 0;

	for (int i = 0; i < 3; i++)
	{
		x += relativePoints[i].x;
		y += relativePoints[i].y;
	}

	vec2 mid(x / 3, y / 3);

	for (int i = 0; i < 3; i++)
	{
		relativePoints[i] -= mid;
	}

	updateTriangle();
}

void Triangle::updateTriangle()
{
	for (int i = 0; i < 3; i++)
	{
		relativePoints[i] = rotate_vector(relativePoints[i], angMomentum);
		points[i] = relativePoints[i] +  position;
		triangle[i].position = vec2_2_sfVec2(&points[i]);
	}
}


void Triangle::physicsStep()
{
	if (!step_calculated)
	{
		//angMomentum *= AIR_RESSISTANCE;
		
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

		

		updateTriangle();


		for (int i = 0; i < 3; i++)
		{
			if (points[i].y > SCREEN_HEIGHT)
			{
				
				

				vec2 contactVector = points[i] - moveDir;
				float contactAngle = atan2(contactVector.y, contactVector.x);

				// Update the angular momentum
				angMomentum = bouncyness * contactAngle * ANG_MOMENTUM_MULTIPLY * angMomentum*10 + moveDir.y/2000 * contactAngle;

				if (points[i].x > position.x && angMomentum > 0)
				{
					angMomentum *= -1;
				}
				if (points[i].x < position.x && angMomentum < 0)
				{
					angMomentum *= -1;
				}


				std::cout << angMomentum << std::endl;
				std::cout << contactAngle << std::endl << std::endl;

				position.y -= points[i].y - SCREEN_HEIGHT;
				moveDir.y *= -bouncyness;


				updateTriangle();
			}

			if (points[i].x < 0)
			{
				vec2 contactVector = points[i] - moveDir;
				float contactAngle = atan2(contactVector.y, contactVector.x);

				// Update the angular momentum
				angMomentum = bouncyness * contactAngle * ANG_MOMENTUM_MULTIPLY * angMomentum * 10 + moveDir.x / 2000 * contactAngle;

				if (points[i].y > position.y && angMomentum > 0)
				{
					angMomentum *= -1;
				}
				if (points[i].y < position.y && angMomentum < 0)
				{
					angMomentum *= -1;
				}


				std::cout << angMomentum << std::endl;
				std::cout << contactAngle << std::endl << std::endl;




				position.x -= points[i].x;
				moveDir.x *= -bouncyness;
				updateTriangle();
			}
			else if (points[i].x > SCREEN_WIDTH)
			{

				vec2 contactVector = points[i] - moveDir;
				float contactAngle = atan2(contactVector.y, contactVector.x);

				// Update the angular momentum
				angMomentum = bouncyness * contactAngle * ANG_MOMENTUM_MULTIPLY * angMomentum * 10 + moveDir.x / 2000 * contactAngle;

				if (points[i].y > position.y && angMomentum < 0)
				{
					angMomentum *= -1;
				}
				if (points[i].y < position.y && angMomentum > 0)
				{
					angMomentum *= -1;
				}


				std::cout << angMomentum << std::endl;
				std::cout << contactAngle << std::endl << std::endl;
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

