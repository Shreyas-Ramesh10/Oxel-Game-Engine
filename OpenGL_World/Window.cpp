#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::initialize()
{
	//Initialisation of GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialisation failed!");
		glfwTerminate();
		return 1;
	};

	//Setup GLFW windows props
	//OpenGL Ver
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No Backwards Compatibility no depricated features
	// Allow Forward Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test WIndow", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	//Get Buffer size info
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // Get frame buffer size from mainWindow and put that in bufferWidth and bufferHeight

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow); //Everything openGL is drawing to should be drawn to mainWindow, can switch multi windows and Switch between them

	//-------------------------------------------------------------------------------------------------------------

	//Allow modern ext features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed macha");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	};

	glEnable(GL_DEPTH_TEST); //Enabling depth test, will make sure the objects are drawn in the correct order, if not it will discard the object that is behind the other object

	//Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
}
Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}