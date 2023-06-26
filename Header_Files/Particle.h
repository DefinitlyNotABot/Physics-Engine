#pragma once
#include "../Header_Files/defines.h"
#include "PhysicsObject.h"


class Particle : public PhysicsObject
{
public:
	Particle(vec2 pos, double rad, sfCol col, bool hollow, float bouncyness, float m);
	void draw(sf::RenderWindow* window) override;
	void physicsStep(int chunk_id) override;

	void collision(const PhysicsObject& p) override;
	void collisionUpdatePos() override;

private:
	void createDot();


	
	
};