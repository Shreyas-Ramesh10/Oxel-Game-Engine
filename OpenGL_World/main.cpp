#define STB_IMAGE_IMPLEMENTATION //Setting the STB to read the header as a code file as supposed to header.

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<cmath>
#include<vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

//Values stored in the GPU memory, we need to convert them to float values and glm::value_ptr will do that for us, 
//in a sense they are not fit to be passed to the shader as raw value we need to have a pointer to the location there.
#include<glm/gtc/type_ptr.hpp> 

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
/* new way to intitialise identity matrix
glm::mat4 model(1.0f);
glm::mat4 model = glm::mat4(1.0f);
model = glm::mat4(1.0f);
*/

const float toRadians = 3.14159265359f / 180.0f; //Converting degrees to radians

Window mainWindow2;
Camera camera;


std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;


// Vertex Shader
static const char* vShader = "Shaders/shader_vert.glsl.txt";

// Fragment Shader
static const char* fShader = "Shaders/shader_frag.glsl.txt";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
	unsigned int vLength, unsigned int normalOffset) 
{
	for (size_t i = 0; i < indiceCount; i += 3) 
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in2] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2); // /\ Normal will give the line poking out of the triangle, either towards us or away from us.
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticeCount / vLength; i++) 
	{
		unsigned int normOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[normOffset], vertices[normOffset + 1], vertices[normOffset + 2]);
		vec = glm::normalize(vec);
		vertices[normOffset] = vec.x; vertices[normOffset + 1] = vec.y; vertices[normOffset + 2] = vec.z;
	}
};

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1 ,3, 2,
		2, 3, 0,
		0, 1, 2,
	};
	GLfloat vertices[] = {
	//    x      y     z      u     v (height)  normX normY normZ
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,        0.0f, 0.0f, 0.0f,// Bottom Left
		 0.0f, -1.0f, 1.0f,	 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,// Background
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,		0.0f, 0.0f, 0.0f,// Bottom Right
		 0.0f,  1.0f, 0.0f,  0.5f, 1.0f,		0.0f, 0.0f, 0.0f // Top
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
}
 
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(*shader1);
};
//-------------------------------------------------------------------------------------------------------------
int main()
{
	mainWindow2 = Window(800, 600);
	if (mainWindow2.initialize() != 0) {
		printf("Failed to initialize window.\n");
		return -1;
	}

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	brickTexture.UseTexture();

	mainLight = Light(1.0f, 1.0f,1.0f,0.2f, 
					2.0f, -1.0f, -2.0f, 1.0f);

	/*GLint MaxUniforms;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &MaxUniforms);
	printf("Max uniforms in vertex shader: %d\n", MaxUniforms);

	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	printf("Max texture units in fragment shader: %d\n", maxTextureUnits);*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0, uniformDirection = 0 ,uniformDiffuseIntensity = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow2.getBufferWidth() / (GLfloat)mainWindow2.getBufferHeight(), 0.1f, 100.0f);

	//Loop it until the window closes
	while (!mainWindow2.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;


		//Get and handle the user input evets
		glfwPollEvents();

		camera.keyControl(mainWindow2.getKeys(), deltaTime);
		camera.mouseControl(mainWindow2.getXChange(), mainWindow2.getYChange());

		//printf("Current Angle: %f\n", currentAngle);
		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

		glm::mat4 model(1.0f);//Will set "model" as an identity matrix

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));//Changing the model matrix values by translating it by the value of triOffset
		//model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//Roating the model matrix by 45 degrees around the z axis
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh(); //Drawing the object

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh(); //Drawing the object

		glUseProgram(0);

		mainWindow2.swapBuffers();//Two scenes will be worked on, one will be displayed and one will be worked on and swaps displayed on with worked on one. 
	}
	return 0;
};



//OpenGL_World