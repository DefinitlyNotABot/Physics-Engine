#include <iostream>
#include <cmath>

#include "../Header_Files/Triangle.h"



sfCol bgCol3 = sfCol(0, 0, 0);

Triangle::Triangle(vec2 pos, vec2 pts[3], sfCol col, bool isHollow, float bouncy, float m)
{
	for (int i = 0; i < 3; i++) {
		relativePoints[i] = pts[i];
	}

	position = pos;
	color = col;
	hollow = isHollow;
	bouncyness = bouncy;
	createTriangle();

	rotationSpeed = 0.0;

	mass = m;

	moveDir = vec2(0, 0);

	ID = nextID;
	nextID++;

	t0 = Time::now();
	step_calculated = false;
	type = PH_TRI;
}

void Triangle::draw(sf::RenderWindow* window)
{
	window->draw(triangle);

}

void Triangle::createTriangle()
{
	triangle = sf::VertexArray(sf::Triangles, 3);

	int x = 0;
	int y = 0;

	for (int i = 0; i < 3; i++)
	{
		x += relativePoints[i].x;
		y += relativePoints[i].y;
	}

	vec2 mid(x / 3, y / 3);

	for (int i = 0; i < 3; i++)
	{
		relativePoints[i] -= mid;
	}for (int i = 0; i < 3; i++)
	{
		relativePoints[i] = rotate_vector(relativePoints[i], 1.5);
		points[i] = relativePoints[i] + position;
		triangle[i].position = vec2_2_sfVec2(&points[i]);
	}

	updateTriangle();
}

void Triangle::updateTriangle()
{
	rotation += rotationSpeed * delta;
	for (int i = 0; i < 3; i++)
	{
		relativePoints[i] = rotate_vector(relativePoints[i], rotationSpeed * delta);
		points[i] = relativePoints[i] + position;
		triangle[i].position = vec2_2_sfVec2(&points[i]);
	}
}


void Triangle::physicsStep(int chunk_id)
{
	if (!step_calculated)
	{
		vec2 oldMoveDir = moveDir;
		vec2 oldPos = position;

		step_calculated = true;

		t1 = Time::now();
		fsec fs = t1 - t0;
		float delta = fs.count();
		this->delta = delta;

		moveDir.y += g * delta;


		float force_length = glm::length(force);
		if (force_length > 0)
		{
			float a = force_length / mass;
			moveDir += glm::normalize(force) * a * delta;
		}


		position += moveDir;


		updateTriangle();

		int num_floor = 0;

		for (int i = 0; i < 3; i++)
		{
			if (on_ground)
			{
				float overlap = points[i].y - SCREEN_HEIGHT;
				position.y -= overlap;
				moveDir.y = 0;
				rotationSpeed = 0;
				updateTriangle();
			}
			else if (points[i].y > SCREEN_HEIGHT)
			{

				float overlap = points[i].y - SCREEN_HEIGHT;
				position.y -= overlap;
				moveDir.y *= -bouncyness;
				updateTriangle();

				calc_rotation(vec2(0, 1), points[i], relativePoints[i]);
			}
			if (points[i].x < 0)
			{
				float overlap = points[i].x - 0;
				position.x -= overlap;
				moveDir.x *= -bouncyness;
				updateTriangle();

				calc_rotation(vec2(1, 0), points[i], relativePoints[i]);
			}
			if (points[i].x > SCREEN_WIDTH)
			{
				float overlap = points[i].x - SCREEN_WIDTH;
				position.x -= overlap;
				moveDir.x *= -bouncyness;
				updateTriangle();

				calc_rotation(vec2(-1, 0), points[i], relativePoints[i]);
			}
			if (points[i].y > SCREEN_HEIGHT - 1.0f)
			{
				num_floor++;
			}
			
		}

		if (glm::abs(moveDir.y) < 0.01f && num_floor == 2)
		{
			moveDir.y = 0.0f;
			position.y = SCREEN_HEIGHT;
			rotationSpeed = 0;
			on_ground = true;
		}



		center = position;

		updateEnergy();

		moveDirSave = moveDir;
		positionSave = position;




		t0 = Time::now();
	}

}

void Triangle::collision(const PhysicsObject& p)
{
	// Calculate the distance between the centers of the two particles
	switch (p.type)
	{
	case PH_PAR:
		break;
	case 0:
	{
		bool found = (std::find(interacted_objects.begin(), interacted_objects.end(), p.ID) != interacted_objects.end());
		if (!found)
		{
			for (int i = 0; i < 3; i++)
			{
				if (PointInTriangle(points[i], p.points[0], p.points[1], p.points[2]))
				{
					std::cout << "intersecting" << std::endl;

					vec2 floorNormal = vec2(0, 1);
					vec2 intersect = vec2(0, 0);

					float dp0 = glm::length(position - p.points[0]);
					float dp1 = glm::length(position - p.points[1]);
					float dp2 = glm::length(position - p.points[2]);


					if (dp0 == std::max(dp0, std::max(dp1, dp2)))
					{
						floorNormal = rotate_vector(glm::normalize(p.points[1] - p.points[2]), PI / 2);
						if (glm::length(position - floorNormal) > glm::length(position - rotate_vector(floorNormal, PI)))
						{
							floorNormal = rotate_vector(floorNormal, PI);
						}
						intersect = calculateIntersection(center, points[i], p.points[1], p.points[2]);
					}
					else if (dp1 == std::max(dp0, std::max(dp1, dp2)))
					{
						floorNormal = rotate_vector(glm::normalize(p.points[0] - p.points[2]), PI / 2);
						if (glm::length(position - floorNormal) > glm::length(position - rotate_vector(floorNormal, PI)))
						{
							floorNormal = rotate_vector(floorNormal, PI);
						}
						intersect = calculateIntersection(center, points[i], p.points[0], p.points[2]);
					}
					else if (dp2 == std::max(dp0, std::max(dp1, dp2)))
					{
						floorNormal = rotate_vector(glm::normalize(p.points[0] - p.points[1]), PI / 2);
						if (glm::length(position - floorNormal) > glm::length(position - rotate_vector(floorNormal, PI)))
						{
							floorNormal = rotate_vector(floorNormal, PI);
						}
						intersect = calculateIntersection(center, points[i], p.points[0], p.points[1]);
					}


					vec2 contactPoint = points[i];

					vec2 r = contactPoint - position;

					// Calculate relative velocities
					vec2 relativeVelocity = vec2(0, moveDir.y);
					float relativeRotation = rotation;

					// Calculate mass inverses
					float triangleMassInverse = 1.0f / mass;
					float triangle2MassInverse = 1 / p.mass;

					// Calculate impulse
					float impulse = -(1 + bouncyness) * glm::dot(relativeVelocity, floorNormal / (triangleMassInverse + triangle2MassInverse));


					// Apply impulse to linear velocity
					moveDir = glm::normalize(moveDir) * ((glm::length(moveDir) + impulse * triangleMassInverse * floorNormal) * bouncyness);

					// Apply impulse to angular velocity
					float a = (glm::length(relativePoints[0]) + glm::length(relativePoints[1]) + glm::length(relativePoints[2])) / 3;
					rotationSpeed += impulse * (1.0f / (mass / 6) * (a * a)) * cross_2D(r, floorNormal) * ANG_MOMENTUM_MULTIPLY;


					moveDir = rotate_vector(moveDir, PI + ang_between_vec(moveDir, relativePoints[i], ANG_CLOCK_SIGNED));




					vec2 overlap = points[i] - intersect;

					positionSave -= overlap;



					updateTriangle();

					interacted_objects.push_back(p.ID);
				}



				//if (PointInTriangle(p.points[i], points[0], points[1], points[2]))
				//{

				//	vec2 floorNormal = vec2(0, 0);
				//	vec2 side = vec2(0, 0);

				//	float d0 = glm::length(p.center - points[0]);
				//	float d1 = glm::length(p.center - points[1]);
				//	float d2 = glm::length(p.center - points[2]);



				//	float longest = std::max(d0, std::max(d1, d2));


				//	if (d0 == longest)
				//	{
				//		side = points[1] - points[2];
				//		floorNormal = glm::normalize(rotate_vector(side, PI / 2));
				//		if (ang_between_vec(floorNormal, center - p.points[0], ANG_ABSOLUTE) < PI / 2)
				//		{
				//			floorNormal = rotate_vector(floorNormal, PI);
				//		}
				//	}
				//	else if (d1 == longest)
				//	{
				//		side = points[0] - points[2];
				//		floorNormal = glm::normalize(rotate_vector(side, PI / 2));
				//		if (ang_between_vec(floorNormal, center - p.points[1], ANG_ABSOLUTE) < PI / 2)
				//		{
				//			floorNormal = rotate_vector(floorNormal, PI);
				//		}
				//	}
				//	else if (d2 == longest)
				//	{
				//		side = points[0] - points[1];
				//		floorNormal = glm::normalize(rotate_vector(side, PI / 2));
				//		if (ang_between_vec(floorNormal, center - p.points[2], ANG_ABSOLUTE) < PI / 2)
				//		{
				//			floorNormal = rotate_vector(floorNormal, PI);
				//		}
				//	}




				//	vec2 contactPoint = points[i];

				//	vec2 r = contactPoint - position;

				//	// Calculate relative velocities
				//	vec2 relativeVelocity = vec2(0, moveDir.y);
				//	float relativeRotation = rotation;

				//	// Calculate mass inverses
				//	float triangleMassInverse = 1.0f / mass;
				//	float floorMassInverse = 1 / p.mass;

				//	// Calculate impulse
				//	float impulse = -(1 + bouncyness) * glm::dot(relativeVelocity, floorNormal / (triangleMassInverse + floorMassInverse));


				//	// Apply impulse to linear velocity
				//	moveDirSave = glm::normalize(moveDir) * ((glm::length(moveDir) + impulse * triangleMassInverse * floorNormal) * bouncyness);

				//	// Apply impulse to angular velocity
				//	float a = (glm::length(relativePoints[0]) + glm::length(relativePoints[1]) + glm::length(relativePoints[2])) / 3;
				//	rotationSpeed += impulse * (1.0f / (mass / 6) * (a * a)) * cross_2D(r, floorNormal) * ANG_MOMENTUM_MULTIPLY;


				//	moveDirSave = rotate_vector(moveDir, PI + ang_between_vec(moveDir, relativePoints[i], ANG_CLOCK_SIGNED));


				//	vec2 intersect = vec2(0, 0);

				//	if (d0 == longest)
				//	{

				//		intersect = calculateIntersection(center, points[i], p.points[1], p.points[2]);
				//	}
				//	else if (d1 == longest)
				//	{
				//		intersect = calculateIntersection(center, points[i], p.points[0], p.points[2]);
				//	}
				//	else if (d2 == longest)
				//	{
				//		intersect = calculateIntersection(center, points[i], p.points[0], p.points[1]);
				//	}


				//	vec2 overlap = points[i] - intersect;

				//	positionSave += overlap;



				//	updateTriangle();
				//	//interacted_objects.push_back(p.ID);
				//}

			}
		}



	}
	break;
	}

}

void Triangle::collisionUpdatePos()
{
	position = positionSave;
	moveDir = moveDirSave;
}

void Triangle::calc_rotation(vec2 collision_normal, vec2 point, vec2 relativePoint)
{
	vec2 anti_normal = rotate_vector(collision_normal, PI);

	float ang_anor_move = ang_between_vec(anti_normal, moveDir, ANG_CLOCK_SIGNED);
	float ang_anor_point = ang_between_vec(anti_normal, relativePoint, ANG_CLOCK_SIGNED);



	float v_point = (2 * PI * glm::length(relativePoint)) * rotationSpeed;
	vec2 pointMoveDir;
	if (rotationSpeed > 0)
	{
		pointMoveDir = v_point * glm::normalize(rotate_vector(relativePoint, PI / 2));
	}
	else
	{
		pointMoveDir = v_point * glm::normalize(rotate_vector(relativePoint, -PI / 2));
	}
	pointMoveDir += moveDir;


	vec2 impact_vect = -pointMoveDir;

	vec2 x = relativePoint - (glm::dot(relativePoint, impact_vect) / glm::length(impact_vect)) * impact_vect;

	float anti_rotation = glm::length(x) * glm::length(relativePoint) * bouncyness * ANG_MOMENTUM_MULTIPLY;


	if (ang_anor_move > ang_anor_point)
	{
		rotationSpeed += anti_rotation;
	}
	else 
	{
		rotationSpeed -= anti_rotation;
	}


}