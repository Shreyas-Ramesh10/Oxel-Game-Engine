#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);
	int initialize();

	GLfloat getBufferWidth() { return bufferWidth; };
	GLfloat getBufferHeight() { return bufferHeight; };

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow);  };

	bool* getKeys() { return keys; };
	GLfloat getXChange();
	GLfloat getYChange();


	void swapBuffers() { glfwSwapBuffers(mainWindow); };

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange = 0.0f;
	GLfloat yChange = 0.0f;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

};

