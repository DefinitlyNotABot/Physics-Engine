#pragma once
#include "../Header_Files/defines.h"
#include "PhysicsObject.h"


class Triangle : public PhysicsObject
{
public:
	Triangle(vec2 pos, vec2 pts[3], sfCol col, bool hollow, float bouncyness, float m);
	void draw(sf::RenderWindow* window) override;
	void physicsStep(int chunk_id) override;

	void collision(PhysicsObject& p) override;
	void collisionUpdatePos() override;
	void updateTriangle();



	

private:
	void createTriangle();
	void calc_rotation(vec2 collision_normal, vec2 point, vec2 relativePoint);
	void calc_rotation_intersect(vec2 point, vec2 relativePoint, PhysicsObject& p1, PhysicsObject& p2);
	void check_for_nan_vals();
	void floor_check();

};