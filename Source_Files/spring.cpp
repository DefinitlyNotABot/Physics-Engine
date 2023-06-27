#include <iostream>
#include <cmath>

#include "../Header_Files/spring.h"

int Spring::nextID = 0;

Spring::Spring(Masspoint* a, Masspoint* b, float stiff, float damp)
{
	A = a;
	B = b;
	stiffness = stiff;
	damping = damp;
	rest_length = glm::length(A->position - B->position);
	force = 0;

	prev_ab = vec2(0, 0);

	ID = nextID;
	nextID++;
}

Spring::Spring(){}
void Spring::draw(sf::RenderWindow* window)
{
	line[0].position = vec2_2_sfVec2(&A->position);
	line[1].position = vec2_2_sfVec2(&B->position);

	window->draw(line);
}

void Spring::physicsStep()
{
	vec2 ap = A->position;
	vec2 bp = B->position;
	if (ap != bp)
	{
		float forwardForce = stiffness * (glm::length(bp - ap) - rest_length);

		vec2 dirAB = glm::normalize(bp - ap);
		vec2 vel = B->velocity - A->velocity;
		float dot = glm::dot(dirAB, vel);

		float dampingForce = dot * damping;

		force = forwardForce + dampingForce;


		vec2 forceA = force * glm::normalize(bp - ap);
		vec2 forceB = force * glm::normalize(ap - bp);
		A->addForce(forceA);
		B->addForce(forceB);
		prev_ab = dirAB;
		
	}
	else{
		A->position -= prev_ab * A->radius;
		B->position += prev_ab * B->radius;
	}
}


