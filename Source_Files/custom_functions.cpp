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