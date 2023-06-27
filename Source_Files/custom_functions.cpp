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



bool checkForNaN(const char* varName, float value)
{
	if (std::isnan(value) || std::isnan(-value))
	{
		std::cout << "NaN detected in variable: " << varName << std::endl;
		return true;
	}
	return false;
}

int orientation(vec2 p, vec2 q, vec2 r) {
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (val == 0)
		return 0;  // collinear
	return (val > 0) ? 1 : 2;  // clockwise or counterclockwise
}

// Function to check if two line segments intersect
bool doIntersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2) {
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1, and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && orientation(p1, p2, q1) == 0)
		return true;

	// p1, q1, and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && orientation(p1, q2, q1) == 0)
		return true;

	// p2, q2, and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && orientation(p2, p1, q2) == 0)
		return true;

	// p2, q2, and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && orientation(p2, q1, q2) == 0)
		return true;

	return false;  // No intersection
}


float normalDistanceToLine(vec2 P, vec2 L1, vec2 L2)
{
	float dividend = abs((L2.x - L1.x) * (L1.y - P.y) - (L1.x - P.x) * (L2.y - L1.y));
	float divisor = sqrt(pow(L2.x - L1.x, 2) + pow(L2.y - L1.y, 2));

	if (divisor != 0.f) 
	{
		return dividend / divisor;
	}
	return sqrt(pow(P.x - L1.x, 2) + pow(P.y - L1.y, 2));
}

