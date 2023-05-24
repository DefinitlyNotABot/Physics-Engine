#pragma once
#include "../Header_Files/defines.h"

class Triangle
{
public:
	Triangle();

	void rotate(float deg);
	void move(float x, float y);

private:

	sf::ConvexShape triangle;



};