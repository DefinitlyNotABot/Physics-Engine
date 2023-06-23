#pragma once
#include "../Header_Files/defines.h"
#include "PhysicsObject.h"


class Triangle : public PhysicsObject
{
public:
	Triangle(vec2 pos, vec2 pts[3], sfCol col, bool hollow, float bouncyness, float m);
	void draw(sf::RenderWindow* window) override;
	void physicsStep() override;

	void collision(const PhysicsObject& p) override;
	void collisionUpdatePos() override;
	void updateTriangle();



	

private:
	void createTriangle();

	

};