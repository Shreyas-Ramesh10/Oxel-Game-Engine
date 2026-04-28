#include "Light.h"

Light::Light()
{
	//White light in this RGB, we use 1.0f as it shows how much of that color should be shown.
	color = glm::vec3(1.0f, 1.0f, 1.0f); 
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
};

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
};
/*
void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
*/

Light::~Light() {};