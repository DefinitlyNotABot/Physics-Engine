#include "../Header_Files/defines.h"


sf::Vector2f vec2_2_sfVec2(vec2* x)
{
	return sf::Vector2f(x->x, x->y);
}

vec2 rotate_vector(vec2 v, float ang)
{
	glm::mat2 matrix(cos(ang), -sin(ang), sin(ang), cos(ang));

	return v * matrix;
}



float cross_2D(vec2 a, vec2 b) {
	return (a.x * b.y - a.y * b.x);
}




float ang_between_vec(vec2 a, vec2 b, int type)
{
	float d;
	float c;
	switch (type)
	{
	case ANG_ABSOLUTE:
		return acos(glm::dot(a, b) / (glm::length(a) * glm::length(b)));

	case ANG_CLOCK_SIGNED:
		d = glm::dot(a, b);
		c = cross_2D(a, b);
		return atan2(c, d);


	default:
		return NULL;
	}

}


float sign(vec2 p1, vec2 p2, vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(vec2 pt, vec2 v1, vec2 v2, vec2 v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}


vec2 calculateIntersection(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4) {
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;
	float x3 = p3.x;
	float y3 = p3.y;
	float x4 = p4.x;
	float y4 = p4.y;

	// Calculate the determinant
	float det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	// Calculate the intersection point
	float intersectionX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / det;
	float intersectionY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / det;

	return vec2(intersectionX, intersectionY);
}