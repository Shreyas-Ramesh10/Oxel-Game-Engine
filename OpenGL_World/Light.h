#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);//aIntensity = ambient intensity

	//Locations in GPU to add these values to shader || will be overwridden for point and spot light
	//void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation);

	~Light();

protected:
	glm::vec3 color; //rgb color
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};