#define GLFW_INCLUDE_NONE

#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"
#include <glad/glad.h>

int OpenGLVersion;

void init() {
	if (!glfwInit())
		throw ("Unable to init GLFW");
}

void openGLContextInit() {
	OpenGLVersion = gladLoadGL();
	if (OpenGLVersion == 0)
		throw ("Failed to load GLAD");
}

void deinit() {
	glfwTerminate();
}

int main() {

	init();

	Window* window = new Window(640, 480, "Test Window");

	window->beginRender();
	openGLContextInit();

	while (!window->isClosing()) {
		glClear(GL_COLOR_BUFFER_BIT);
		window->endRender();
		glfwPollEvents();
	}

	deinit();

	return 0;
}