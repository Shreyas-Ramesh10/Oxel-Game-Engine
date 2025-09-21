#pragma once

#include <GL/glew.h>
class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint SpecularIntensityLocation, GLuint ShininessLocation);

	~Material();

private:
	GLfloat specularIntensity; //How bright the light is
	GLfloat shininess; // How collected the light is
};

