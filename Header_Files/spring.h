#pragma once
#include "../Header_Files/defines.h"
#include "../Header_Files/masspoint.h"

class Spring
{
public:
	Spring(Masspoint* a, Masspoint* b, float stiffness, float damping);
	void draw(sf::RenderWindow* window);

	void physicsStep();

	static int nextID;
	int ID = 0;

	Masspoint* A;
	Masspoint* B;
	float stiffness;
	float rest_length;
	float damping;
	float force;

	vec2 prev_ab;

	sf::VertexArray line = sf::VertexArray(sf::LinesStrip, 2);
};