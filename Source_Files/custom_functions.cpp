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

