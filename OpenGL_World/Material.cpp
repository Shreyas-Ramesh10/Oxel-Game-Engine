#include "Material.h"

Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint SpecularIntensityLocation, GLuint ShininessLocation)
{
	glUniform1f(SpecularIntensityLocation, specularIntensity);
	glUniform1f(ShininessLocation, shininess);
}

Material::~Material()
{

}