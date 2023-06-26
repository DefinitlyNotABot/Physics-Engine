#include <iostream>
#include <cmath>

#include "../Header_Files/Triangle.h"
std::mutex mtx;


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

	triangle[0].color = col;
	triangle[1].color = col;
	triangle[2].color = col;


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
	//std::cout << "Drawing triangle " << ID << " " << position.x << "   " << position.y << std::endl;
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
	}

	for (int i = 0; i < 3; i++)
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
		if (glm::length(relativePoints[i]) < 10)
		{
			std::cout << "len " << ID << " " << glm::length(relativePoints[i]) << std::endl;
		}

	}
}


void Triangle::physicsStep(int chunk_id)
{
	mtx.lock();
	if (!step_calculated)
	{



		vec2 oldMoveDir = moveDir;
		vec2 oldPos = position;

		step_calculated = true;

		t1 = Time::now();
		fsec fs = t1 - t0;
		float delta = fs.count();


		if (DEBUG_SINGLESTEP)
		{
			delta = DEBUG_SINGLESTEP_DELTATIME;
		}


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

		floor_check();

		check_for_nan_vals();

		center = position;

		updateEnergy();

		moveDirSave = moveDir;
		positionSave = position;




		t0 = Time::now();
	}
	mtx.unlock();

}

void Triangle::collision(PhysicsObject& p)
{
	mtx.lock();
	// Calculate the distance between the centers of the two particles
	switch (p.type)
	{
	case PH_PAR:
		break;
	case PH_TRI:
	{
		bool found = (std::find(interacted_objects.begin(), interacted_objects.end(), p.ID) != interacted_objects.end());
		if (!found)
		{
			for (int i = 0; i < 3; i++)
			{
				if (PointInTriangle(points[i], p.points[0], p.points[1], p.points[2]))
				{

					calc_rotation_intersect(points[i], relativePoints[i], *this, p);


					on_ground = false;

					interacted_objects.push_back(p.ID);
				}



				else if (PointInTriangle(p.points[i], points[0], points[1], points[2]))
				{

					calc_rotation_intersect(points[i], relativePoints[i], *this, p);
					on_ground = false;
					interacted_objects.push_back(p.ID);
				}



			}

			bool found = (std::find(interacted_objects.begin(), interacted_objects.end(), p.ID) != interacted_objects.end());
			if (!true)
			{
				for (int i = 0; i < 3; i++) {
					if (std::find(interacted_objects.begin(), interacted_objects.end(), p.ID) != interacted_objects.end())
					{
						break;
					}
					if (doIntersect(points[i], points[(i + 1) % 3], p.points[0], points[1])) {
						std::cout << "210" << std::endl;
						vec2 point = calculateIntersection(points[i], points[(i + 1) % 3], p.points[0], points[1]);
						vec2 relPoint = position - point;
						calc_rotation_intersect(point, relPoint, p, *this);
						interacted_objects.push_back(p.ID);
					}
					if (std::find(interacted_objects.begin(), interacted_objects.end(), p.ID) != interacted_objects.end())
					{
						break;
					}
					if (doIntersect(points[i], points[(i + 1) % 3], p.points[1], points[2])) {
						std::cout << "221" << std::endl;
						vec2 point = calculateIntersection(points[i], points[(i + 1) % 3], p.points[1], points[2]);
						vec2 relPoint = position - point;
						calc_rotation_intersect(point, relPoint, p, *this);

						interacted_objects.push_back(p.ID);
					}
					if (std::find(interacted_objects.begin(), interacted_objects.end(), p.ID) != interacted_objects.end())
					{
						break;
					}
					if (doIntersect(points[i], points[(i + 1) % 3], p.points[2], points[0])) {
						std::cout << "233" << std::endl;
						vec2 point = calculateIntersection(points[i], points[(i + 1) % 3], p.points[1], points[2]);
						vec2 relPoint = position - point;
						calc_rotation_intersect(point, relPoint, p, *this);

						interacted_objects.push_back(p.ID);
					}
				}

			}


			floor_check();
		}



	}
	break;
	}


	mtx.unlock();

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

void Triangle::calc_rotation_intersect(vec2 point, vec2 relativePoint, PhysicsObject& p1, PhysicsObject& p2)
{
	vec2 collision_normal = vec2(0, 0);
	vec2 intersect = vec2(0, 0);

	/* Calculate distances between points of triangle 2 and center of triangle 1 */
	float dp0 = glm::length(p1.position - p2.points[0]);
	float dp1 = glm::length(p1.position - p2.points[1]);
	float dp2 = glm::length(p1.position - p2.points[2]);
	float dpMax = std::max(dp0, std::max(dp1, dp2));	// Maximum of the distances

	if (dp0 == dpMax)
	{
		collision_normal = rotate_vector(glm::normalize(p2.points[1] - p2.points[2]), PI / 2);	// Rotate normalized Vector of triangle side by 90°
		if (glm::length(p1.position - collision_normal) > glm::length(p1.position + collision_normal))
		{
			collision_normal = -collision_normal;												// Rotate collision_normal by 180° if necessary
		}
		/* Calculate point of intersection from point and moveDir */
		intersect = calculateIntersection(point - (glm::normalize(p1.moveDir) * dp0), point + (glm::normalize(p1.moveDir) * dp0), p2.points[1], p2.points[2]);
	}
	else if (dp1 == dpMax)
	{
		collision_normal = rotate_vector(glm::normalize(p2.points[0] - p2.points[2]), PI / 2);	// Rotate normalized Vector of triangle side by 90°
		if (glm::length(p1.position - collision_normal) > glm::length(p1.position + collision_normal))
		{
			collision_normal = -collision_normal;												// Rotate collision_normal by 180° if necessary
		}
		/* Calculate point of intersection from point and moveDir */
		intersect = calculateIntersection(point - (glm::normalize(p1.moveDir) * dp0), point + (glm::normalize(p1.moveDir) * dp0), p2.points[0], p2.points[2]);
	}
	else if (dp2 == dpMax)
	{
		collision_normal = rotate_vector(glm::normalize(p2.points[0] - p2.points[1]), PI / 2);	// Rotate normalized Vector of triangle side by 90°
		if (glm::length(p1.position - collision_normal) > glm::length(p1.position + collision_normal))
		{
			collision_normal = -collision_normal;												// Rotate collision_normal by 180° if necessary
		}
		/* Calculate point of intersection from point and moveDir */
		intersect = calculateIntersection(point - (glm::normalize(p1.moveDir) * dp0), point + (glm::normalize(p1.moveDir) * dp0), p2.points[0], p2.points[1]);
	}

	vec2 anti_normal = -collision_normal;	// Get the inverse of the normal
		
	float ang_anor_move = ang_between_vec(anti_normal, p1.moveDir, ANG_CLOCK_SIGNED);		// Get angle between anormal and moveDir
	float ang_anor_point = ang_between_vec(anti_normal, relativePoint, ANG_CLOCK_SIGNED);	// get angle between anormal and the relative Point



	float v_point = (2 * PI * glm::length(relativePoint)) * p1.rotationSpeed;	// get the speed of the intersecting point


	vec2 pointMoveDir;
	if (p1.rotationSpeed > 0)
	{
		pointMoveDir = v_point * glm::normalize(rotate_vector(relativePoint, PI / 2));
	}
	else
	{
		pointMoveDir = v_point * glm::normalize(rotate_vector(relativePoint, -PI / 2));
	}
	pointMoveDir += p1.moveDir;


	vec2 impact_vect = -pointMoveDir;

	vec2 x = relativePoint - (glm::dot(relativePoint, impact_vect) / glm::length(impact_vect)) * impact_vect;

	float anti_rotation = glm::length(x) * glm::length(relativePoint) * p1.bouncyness * ANG_MOMENTUM_MULTIPLY * 0.1 * ((p1.mass + p2.mass) / p1.mass);


	if (ang_anor_move > ang_anor_point)
	{
		p1.rotationSpeed += anti_rotation;
	}
	else
	{
		p1.rotationSpeed -= anti_rotation;
	}

	float triangleMassInverse = 1.0f / p1.mass;
	float triangle2MassInverse = 1.0f / p2.mass;

	vec2 relativeVelocity = p1.moveDir - p2.moveDir;

	// Calculate impulse
	float impulse = -(1 + p1.bouncyness) * glm::dot(relativeVelocity, collision_normal / (triangleMassInverse + triangle2MassInverse));


	// Apply impulse to linear velocity
	p1.moveDir = glm::normalize(p1.moveDir) * ((glm::length(p1.moveDir) + impulse * triangleMassInverse * collision_normal) * p1.bouncyness) * ((p1.mass + p2.mass) / p1.mass);

	// Apply impulse to angular velocity


	p1.moveDir = rotate_vector(p1.moveDir, PI + ang_between_vec(p1.moveDir, relativePoint, ANG_CLOCK_SIGNED));




	vec2 overlap = point - intersect;

	p1.position -= overlap/2.0f;


	p1.positionSave = p1.position;
	p1.moveDirSave = p1.moveDir;
	updateTriangle();
}




void Triangle::check_for_nan_vals()
{
	checkForNaN("position.x", position.x);
	checkForNaN("position.y", position.y);
	checkForNaN("center.x", center.x);
	checkForNaN("center.y", center.y);
	checkForNaN("positionSave.x", positionSave.x);
	checkForNaN("positionSave.y", positionSave.y);
	checkForNaN("force.x", force.x);
	checkForNaN("force.y", force.y);
	checkForNaN("moveDir.x", moveDir.x);
	checkForNaN("moveDir.y", moveDir.y);
	checkForNaN("moveDirSave.x", moveDirSave.x);
	checkForNaN("moveDirSave.y", moveDirSave.y);
	checkForNaN("rotationSpeed", rotationSpeed);
	checkForNaN("relativePoints[0].x", relativePoints[0].x);
	checkForNaN("relativePoints[0].y", relativePoints[0].y);
	checkForNaN("relativePoints[1].x", relativePoints[1].x);
	checkForNaN("relativePoints[1].y", relativePoints[1].y);
	checkForNaN("relativePoints[2].x", relativePoints[2].x);
	checkForNaN("relativePoints[2].y", relativePoints[2].y);
	checkForNaN("points[0].x", points[0].x);
	checkForNaN("points[0].y", points[0].y);
	checkForNaN("points[1].x", points[1].x);
	checkForNaN("points[1].y", points[1].y);
	checkForNaN("points[2].x", points[2].x);
	checkForNaN("points[2].y", points[2].y);
	checkForNaN("rotation", rotation);
	checkForNaN("delta", delta);
}

void Triangle::floor_check()
{
	int num_floor = 0;

	for (int i = 0; i < 3; i++)
	{
		if (on_ground)
		{
			if (points[i].y > SCREEN_HEIGHT)
			{
				float overlap = points[i].y - SCREEN_HEIGHT;
				position.y -= overlap;
			}

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
}