#include <iostream>
#include <cmath>

#include "../Header_Files/spring.h"

Spring::Spring(Masspoint* a, Masspoint* b, float stiff, float damp)
{
	A = a;
	B = b;
	stiffness = stiff;
	damping = damp;
	rest_length = glm::length(A->position - B->position);

}

void Spring::draw(sf::RenderWindow* window)
{
	line[0].position = vec2_2_sfVec2(&A->position);
	line[1].position = vec2_2_sfVec2(&B->position);

	window->draw(line);
}