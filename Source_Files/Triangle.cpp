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

	angMomentum = 0.1;

	mass = m;

	moveDir = vec2(0, 0);

	ID = nextID;
	nextID++;

	t0 = Time::now();
	step_calculated = false;
	type = PH_TRI;
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
	}for (int i = 0; i < 3; i++)
	{
		relativePoints[i] = rotate_vector(relativePoints[i], 1.5);
		points[i] = relativePoints[i] + position;
		triangle[i].position = vec2_2_sfVec2(&points[i]);
	}

	updateTriangle();
}

void Triangle::updateTriangle()
{
	for (int i = 0; i < 3; i++)
	{
		relativePoints[i] = rotate_vector(relativePoints[i], angMomentum * delta);
		points[i] = relativePoints[i] + position;
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
				vec2 floorNormal = vec2(0,1);
				vec2 contactPoint = points[i];

				vec2 r = contactPoint - position;

				// Calculate relative velocities
				vec2 relativeVelocity = moveDir;
				float relativeRotation = angMomentum;

				// Calculate mass inverses
				float triangleMassInverse = 1.0f / mass;
				float floorMassInverse = 1 / FLOOR_MASS;

				// Calculate impulse
				float impulse = -(1 + bouncyness) * glm::dot(relativeVelocity, floorNormal / (triangleMassInverse + floorMassInverse));
					

				// Apply impulse to linear velocity
				moveDir = glm::normalize(moveDir) * ((glm::length(moveDir) + impulse * triangleMassInverse * floorNormal) * bouncyness);

				// Apply impulse to angular velocity
				float a = (glm::length(relativePoints[0]) + glm::length(relativePoints[1]) + glm::length(relativePoints[2]))/3;
				angMomentum += impulse * (1.0f / (mass / 6) * (a * a)) * cross_2D(r, floorNormal) * ANG_MOMENTUM_MULTIPLY;
				

				moveDir = rotate_vector(moveDir, PI + ang_between_vec(moveDir, relativePoints[i], ANG_CLOCK_SIGNED));

				position.y -= points[i].y - SCREEN_HEIGHT;
				updateTriangle();
			}

			if (points[i].x < 0)
			{
				vec2 contactVector = points[i] - moveDir;
				float contactAngle = atan2(contactVector.y, contactVector.x);

				// Update the angular momentum
				angMomentum = bouncyness * contactAngle * ANG_MOMENTUM_MULTIPLY * angMomentum * 10 + glm::length(moveDir) / 2000 * contactAngle;

				if (points[i].y > position.y && angMomentum > 0)
				{
					angMomentum *= -1;
				}
				if (points[i].y < position.y && angMomentum < 0)
				{
					angMomentum *= -1;
				}



				position.x -= points[i].x;
				moveDir.x *= -bouncyness;
				updateTriangle();
			}
			else if (points[i].x > SCREEN_WIDTH)
			{

				vec2 contactVector = points[i] - moveDir;
				float contactAngle = atan2(contactVector.y, contactVector.x);

				// Update the angular momentum
				angMomentum = bouncyness * contactAngle * ANG_MOMENTUM_MULTIPLY * angMomentum * 10 + glm::length(moveDir) / 2000 * contactAngle;

				if (points[i].y > position.y && angMomentum < 0)
				{
					angMomentum *= -1;
				}
				if (points[i].y < position.y && angMomentum > 0)
				{
					angMomentum *= -1;
				}
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

