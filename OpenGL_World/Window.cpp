#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	mouseFirstMoved = true;

	for (size_t i = 0; i < 1024; i++) 
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	mouseFirstMoved = true;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

}

int Window::initialize()
{
	//Initialisation of GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialisation failed!\n");
		glfwTerminate();
		return 1;
	};

	//Setup GLFW window props
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
		printf("GLFW window creation failed!\n");
		glfwTerminate();
		return 1;
	}

	//Get Buffer size info
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // Get frame buffer size from mainWindow and put that in bufferWidth and bufferHeight

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow); //Everything openGL is drawing to should be drawn to mainWindow, can switch multi windows and Switch between them

	// Handle Key + Mouse input
	

	//This hides the cursor and keeps it at the center, which window? what input?(cursor, keys etc), and what should be done to that input(cursor/keys)
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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

	//Setup Viewport size, not belonged to GLFW actual openGL thing x, y co-ords and we need bufferWidth/height which is the section in the middle not the whole window.
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
	createCallbacks();

	printf("Window successfully initialized.\n");
	return 0;
}


void Window::createCallbacks()
{ 
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}
GLfloat Window::getXChange() 
{
	GLfloat theChange = xChange;
	xChange = 0.0f; // Reset xChange after getting it
	return theChange;
};
GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f; // Reset xChange after getting it
	return theChange;
};

//--------------------------------------------------------------------------------------------------------------------------------
//Handle Keys
void Window::handleKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*printf("[Key callback] Got key: %d, action: %d\n", key, action);
	printf("Key Event Triggered: key=%d, action=%d\n", key, action);
	printf("HANDLE KEYS HIT! key = %d, action = %d\n", key, action);*/
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!theWindow) {
		printf(" theWindow is null — user pointer not set properly!\n");
		return;
	}

	//printf("Key: %d, Action: %d\n", key, action);

	//If the user has pressed the Esc key, the window should close. That's why we are checking for both if any KEY is pressed and also if its the ESCAPE key.
	if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_RELEASE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->keys[key] = true; //If key is pressed, set the key to true
			//printf("Pressed: %d\n", key);
		} 
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}

	}

}

//-----------------------------------------------------------------------------------------------------------------------------------

//Handle Mouse movement
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; // Invert Y axis, because in OpenGL Y axis is inverted

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

//-----------------------------------------------------------------------------------------------------------------------------------


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}