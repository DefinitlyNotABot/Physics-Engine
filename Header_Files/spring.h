#pragma once
#include "../Header_Files/defines.h"
#include "../Header_Files/masspoint.h"

class Spring
{
public:
	Spring(Masspoint* a, Masspoint* b, float stiffness, float damping);
	void draw(sf::RenderWindow* window);

	Masspoint* A;
	Masspoint* B;
	float stiffness;
	float rest_length;
	float damping;

	sf::VertexArray line = sf::VertexArray(sf::LinesStrip, 2);
};