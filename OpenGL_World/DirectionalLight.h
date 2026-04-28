#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat aIntensity, GLfloat dIntensity, //The values above the line are set by the super constructor : Light()
					 GLfloat xDir, GLfloat yDir, GLfloat zDir);

	//Locations in GPU to add these values to shader || will be overwridden for point and spot light
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

