#include <iostream>
#include <cmath>

#include "../Header_Files/classes.h"

int Masspoint::nextID = 0;

Masspoint::Masspoint(vec2 pos, float m, float rad)
{
	position = pos;
	velocity = vec2(0, 0);
	force = vec2(0, 0);
	mass = m;
	radius = rad;

	ID = nextID;
	nextID++;

	dot = sf::CircleShape(radius);

}

Masspoint::Masspoint(){}


void Masspoint::draw(sf::RenderWindow* window)
{
	dot.setPosition(position.x - radius, position.y - radius);
	window->draw(dot);
}

void Masspoint::addForce(vec2 f)
{
	force += f;
}

void Masspoint::physicsStep(float delta)

{


	force += vec2(0, g);

	velocity += (delta * force) / mass;
	position += delta * velocity;

	if (position.y > SCREEN_HEIGHT - 2 * radius)
	{
		position.y = SCREEN_HEIGHT - 2 * radius;
		velocity.y *= -1;
	}
	if (position.x > SCREEN_WIDTH - 2 * radius)
	{
		position.x = SCREEN_WIDTH - 2 * radius;
		velocity.x *= -1;
	}
	if (position.x < 2 * radius)
	{
		position.x = 2 * radius;
		velocity.x *= -1;
	}


}

void Masspoint::reset()
{
	force = vec2(0, 0);
}

void Masspoint::collision(PhysicsObject& p)
{
	switch (p.type)
	{
	case PH_PAR:
	{

	}
	break;
	case PH_TRI:
	{
		vec2 center = vec2(position.x - radius, position.y - radius);
		if (PointInTriangle(center, p.points[0], p.points[1], p.points[2]))
		{
			float d1 = normalDistanceToLine(center, p.points[0], p.points[1]);
			float d2 = normalDistanceToLine(center, p.points[1], p.points[2]);
			float d3 = normalDistanceToLine(center, p.points[2], p.points[0]);
			float minD = std::min(d1, std::min(d2, d3));

			vec2 normal = vec2(0, 0);

			if (d1 == minD)
			{
				normal = rotate_vector(glm::normalize(p.points[0] - p.points[1]), PI / 2);	// Rotate normalized Vector of triangle side by 90°
				if (glm::length(center - normal) > glm::length(center + normal))
				{
					normal = -normal;												// Rotate collision_normal by 180° if necessary
				}
			}
			else if (d2 == minD)
			{
				normal = rotate_vector(glm::normalize(p.points[1] - p.points[2]), PI / 2);	// Rotate normalized Vector of triangle side by 90°
				if (glm::length(center - normal) > glm::length(center + normal))
				{
					normal = -normal;												// Rotate collision_normal by 180° if necessary
				}
			}
			else if (d3 == minD)
			{
				normal = rotate_vector(glm::normalize(p.points[2] - p.points[0]), PI / 2);	// Rotate normalized Vector of triangle side by 90°
				if (glm::length(center - normal) > glm::length(center + normal))
				{
					normal = -normal;												// Rotate collision_normal by 180° if necessary
				}
			}


			position -= normal * minD;

			velocity = velocity - 2 * glm::dot(velocity, normal) * normal;

		}


	}
	break;
	case PH_SOF:
	{

	}
	break;
	}
}


