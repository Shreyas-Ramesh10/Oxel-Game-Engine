#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);//aIntensity = ambient intensity

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation);//will be overwridden for point and spot light

	~Light();

private:
	glm::vec3 color; //rgb color
	GLfloat ambientIntensity;
};