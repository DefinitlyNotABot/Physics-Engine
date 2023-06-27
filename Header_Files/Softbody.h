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


	std::list<Masspoint*> masspoints;
	std::list<Spring*> springs;

	std::chrono::steady_clock::time_point t0;
	std::chrono::steady_clock::time_point t1;
	

private:
	void createSoftbody();
};