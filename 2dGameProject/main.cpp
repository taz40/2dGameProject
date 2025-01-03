#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"

void init() {
	if (!glfwInit())
		throw ("Unable to init GLFW");
}

void deinit() {
	glfwTerminate();
}

int main() {

	init();

	Window* window = new Window(640, 480, "Test Window");

	window->beginRender();

	while (!window->isClosing()) {
		glClear(GL_COLOR_BUFFER_BIT);
		window->endRender();
		glfwPollEvents();
	}

	deinit();

	return 0;
}