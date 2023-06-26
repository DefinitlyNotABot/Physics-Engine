#pragma once
#include "../Header_Files/defines.h"
#include "../Header_Files/classes.h"

#include "../Header_Files/masspoint.h"
#include "../Header_Files/spring.h"


class Softbody : public PhysicsObject
{
public:
	Softbody(vec2 pos, sfCol col, float stiffness, float masses, int w, int h, int resolution, float damp);
	void draw(sf::RenderWindow* window) override;
	void physicsStep(int chunk_id) override;

	void collision(PhysicsObject& p) override;
	void collisionUpdatePos() override;
	void updateTriangle();


	std::list<Masspoint*> masspoints;
	std::list<Spring*> springs;
	

private:
	void createSoftbody();
};